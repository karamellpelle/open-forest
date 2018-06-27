//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "BATB/OGRE.hpp"
#include "BATB/Scene.hpp"
#include "BATB/GL.hpp"
#include "BATB/Screen.hpp"
#include "OgreLogManager.h"
#include "OgreLog.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreCamera.h"
#include "OgreResourceGroupManager.h"
#include "OgreSceneManager.h"
#include "OgreGpuProgramManager.h"
#include "OgreRTShaderSystem.h"
#include "OgreMaterialManager.h"

// custom Shader Generator resolver (for RTSS)
#include "BATB/OGRE/SGTechniqueResolverListener.hpp"

#include "helpers/yaml.hpp"



// macOS
#ifdef BATB_BUILD_PLATFORM_MACOS
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#endif

// linux: X11
// TODO

// linux: WAYLAND
// TODO

#include <GLFW/glfw3native.h>

namespace batb
{

namespace ogre
{



using namespace Ogre;
    
////////////////////////////////////////////////////////////////////////////////
// setup

void OGRE::begin(const std::string& path)
{
    batb->log << "batb->ogre->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // setup Ogre
        // https://ogrecave.github.io/ogre/api/1.11/
        
        // control the log output from Ogre by creating our own LogManager before Root
debug::gl::msg( "OGRE_NEW LogManager" );
        ogre_logmanager = OGRE_NEW Ogre::LogManager();
        auto logpath = file::tmp( "openforest-ogre.log" );
        ogre_logmanager->createLog( logpath, true, false, false );
        ogre_logmanager->setLogDetail( Ogre::LL_BOREME ); // detail level
        batb->log << "Ogre::LogManager writing to " << logpath << std::endl;


        ////////////////////////////////////////////////////////////////////////////////
        // create Root object
debug::gl::msg( "OGRE_NEW Root" );
        ogre_root = OGRE_NEW Ogre::Root( "", "", "" ); // no files for plugin, config, log
        batb->log << "Ogre::Root created" << std::endl;

        ////////////////////////////////////////////////////////////////////////////////
        // add plugins (RenderSystem's, SceneManager's, ...)
        batb->log << "loading plugins:" << std::endl;
        if ( YAML::Node plugins = yaml[ "plugins" ] )
        {
            LogIndent indent( batb->log, "- " );
            
            for (auto i = std::begin( plugins ); i != std::end( plugins ); ++i )
            {
                std::string plugin = i->as<std::string>();
                batb->log << plugin;
                
                try
                {
std::ostringstream os; os << "ogre_root->loadPlugin( " << plugin << ")";                
debug::gl::msg( os.str() );
                    ogre_root->loadPlugin( plugin );
                }
                catch (Ogre::Exception& e)
                {
                    batb->log << " (WARNING: " << e.what() << " )";
                }

                batb->log->endl();
            }
        }
        else
        {
            batb->log << "ERROR: no 'plugins' defined in .yaml" << std::endl;
            throw std::runtime_error( "OGRE: no 'plugins' defined in config" );
        }


        ////////////////////////////////////////////////////////////////////////////////
        // set RenderSystem
        //
        // regarding GL3PlusRenderSystem, see 
        // https://www.khronos.org/opengl/wiki/OpenGL_Context#OpenGL_3.2_and_Profiles
        // https://developer.apple.com/library/content/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/UpdatinganApplicationtoSupportOpenGL3/UpdatinganApplicationtoSupportOpenGL3.html#//apple_ref/doc/uid/TP40001987-CH3-SW1

        // pick RenderSystem defined in yaml. default "OpenGL Rendering Subsystem".
        // TODO: only 3.3
debug::gl::msg( "ogre_root->getRenderSystemByName" );
        ogre_rendersystem_name_ = yaml["rendersystem"].as<std::string>( ogre_rendersystem_name_ );
        ogre_rendersystem = ogre_root->getRenderSystemByName( ogre_rendersystem_name_ );

        // set our Ogre render system
        if ( ogre_rendersystem )
        {
debug::gl::msg( "ogre_root->setRenderSystem" );
            ogre_root->setRenderSystem( ogre_rendersystem );
            batb->log << "RenderSystem used: " << ogre_rendersystem->getName() << std::endl;
        }
        else
        {
            batb->log << "ERROR: no RenderSystem with name '" << ogre_rendersystem_name_ << "'" << std::endl;
            throw std::runtime_error( "OGRE: no RenderSystem with name " + ogre_rendersystem_name_ );
        }

       
        ////////////////////////////////////////////////////////////////////////////////    
        // initialize Root, using our defined RenderSystem
        // (this method returns nullptr, since our argument is 'false')
debug::gl::msg( "ogre_root->initialise" );
        ogre_root->initialise( false ); 
        batb->log << "Ogre::Root initialized" << std::endl;


        ////////////////////////////////////////////////////////////////////////////////
        // create an Ogre window, using our existing GLFW window.
        //
debug::gl::msg( "ogre_root->createRenderWindow()" );
        Ogre::NameValuePairList params;

#ifdef BATB_BUILD_PLATFORM_LINUX
        // TODO: Linux!
#endif

#ifdef BATB_BUILD_PLATFORM_MACOS
        ////////////////////////////////////////////////////////////////////////////////
        // Ogre and macOS, i.e. OSX and Cocoa
        // the interesting code lies in OgreOSXCocoaWindow.mm:CocoaWindow::create() 
        // which handles the given 'miscParams' to createRenderWindow (https://ogrecave.github.io/ogre/api/1.10/class_ogre_1_1_root.html#a537b7d1d0937f799cfe4936f6b672620)
        //
        //
        // GL3PlusRenderSystem < GLRenderSystemCommon:
        //    GLContext* mMainContext     // main thread only
        //    GLContext* mCurrentContext  // main thread only
        //

        // 'id' is defined as void* in glfw3native
        id win = glfwGetCocoaWindow( batb->screen->glfw_window );
        id ctx = glfwGetNSGLContext( batb->screen->glfw_window );

        // let RenderWindow use our NSWindow. without this, Ogre creates a new CocoaWindow :)
        params["externalWindowHandle"] = std::to_string( (unsigned long)( win ) );
        // let RenderWindow use our NSOpenGLContext. 
        params["externalGLContext"] = std::to_string( (unsigned long)( ctx ) );  

#endif
        ////////////////////////////////////////////////////////////////////////////////
        // OgreRoot::createRenderWindow() -> XXXRenderSystem::_createRenderWindow() -> XXXGLSupport::newWindow() 
        //                                -> XXXWindow::create(). and this creates the XXXContext class too.
        ogre_renderwindow = ogre_root->createRenderWindow( "ogre_renderwindow", 800, 600, false, &params );
        // don't let Ogre swap our GLFWwindow; prevent flickering. this should work since 
        // RenderSystem::_swapAllRenderTargetBuffers() only swaps auto updated windows. however, 
        // RenderTarget::setAutoUpdated() and RenderTarget::isAutoUpdated() are virtual functions,
        // so this may not work for all Ogre::XXXWindow's. it works with CocoaWindow.
        ogre_renderwindow->setAutoUpdated( false );
        ogre_renderwindow->setVisible(true);
        batb->log << "RenderWindow created" << std::endl;

        // fix problem with externalWindow handling and resizing. see https://github.com/OGRECave/ogre/issues/774#issuecomment-392755242
        batb->ogre->ogre_renderwindow->windowMovedOrResized();

        ////////////////////////////////////////////////////////////////
        // init various stuff. this may not belong here but we need materials 
        // and RTShaderSystem through the whole open-forest launch (unless we
        // don't use this OGRE Module). it is based upon 
        // Ogre::ApplicationContext::setup()/locateResources()/initialiseRTShaderSystem()
        // of OgreBites.
        //
        // RTShaderSystem
        // see
        //  * https://ogrecave.github.io/ogre/api/1.11/rtss.html
        //  * Ogre::ApplicationContext::locateResources() and Ogre::ApplicationContext::setup() of the OgreBites Component
        //   
        // must be done before loading other resources (i.e. forest::World calling OGRE::addResourceLocation(YAML)),
        // according to Ogre::ApplicationContext::createDummyScene()
        //
        // add material programs:
        initPrograms();
        // init Real Time Shader System
        initRTSS();

        // now init our resource groups as done in ApplicationContext::setup()
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        ////////////////////////////////////////////////////////////////////////////////
        // now emuluate OgreRoot::startRendering(): the closed rendering loop of Ogre.
        // the rest of that functions definition is implemented in frameBegin()/frameEnd()
        // below.
        ogre_root->getRenderSystem()->_initRenderTargets();
        // Clear event times
        ogre_root->clearEventTimes();


        enabled( true );

    }
    ////////////////////////////////////////////////////////////////////////////////
    
    init( true );


    
}


void OGRE::end()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    batb->log << "batb->ogre->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        save();
   
        // needs to be done before modding Ogre, since
        // frameBegin/End may be done in other thread (i.e. main thread)
        // TODO: do this at end to keep invariant of not being initialized and
        //       use a mutex lock instead!
        init( false );
        

        // delete ogre_technique_resolver_listener
        delete ogre_technique_resolver_listener;
        ogre_technique_resolver_listener = nullptr;


debug::gl::msg( "OGRE_DELETE Root" );
        OGRE_DELETE ogre_root;
        batb->log << "Ogre::Root deleted" << std::endl;
debug::gl::msg( "OGRE_DELETE LogManager" );
        OGRE_DELETE ogre_logmanager;
        batb->log << "Ogre::LogManager deleted" << std::endl;

        ogre_root = nullptr;

        // since Ogre steal our GLXContext we have to rebind it after 
        // Ogre shutdown
        //glcontextglfw_.setCurrent();
        //glfwMakeContextCurrent( batb->screen->glfw_window );
    

  
        
        enabled( false );
    }
   
}

////////////////////////////////////////////////////////////////////////////////
// programs

void OGRE::initPrograms()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    batb->log << "initPrograms()" << std::endl;
    LogIndent indent( batb->log, "- " );

    // see Ogre::ApplicationContext::locateResources()
    if ( YAML::Node programs = ( yaml > "materials" > "programs" ) )
    {
        Ogre::String sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

        // according to Ogre::ApplicationContext::locateResources():
        // always add glsl120, add glsl150 if supported or glsl if not, add glsl400 if supported

        // always glsl120 according to Ogre::ApplicationContext::locateResources()
        if ( true )
        {
            batb->log << "adding resource 'glsl120'" << std::endl;
            LogIndent indent( batb->log, "- " );

            addResourceGroup( sec, (programs > "glsl120") ); 
        }

        // glsl400?
        if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400") )
        {
            batb->log << "adding resource 'glsl400'" << std::endl;
            LogIndent indent( batb->log, "- " );

            addResourceGroup( sec, programs > "glsl400" );            
        }

        // glsl150? otherwise glsl
        if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150") )
        {
            batb->log << "adding resource 'glsl150'" << std::endl;
            LogIndent indent( batb->log, "- " );

            addResourceGroup( sec, programs > "glsl150" );            
        }
        else
        {
            batb->log << "adding resource 'glsl'" << std::endl;
            LogIndent indent( batb->log, "- " );

            addResourceGroup( sec, programs > "glsl" );            
        }

    }

}

////////////////////////////////////////////////////////////////////////////////
// RTSS

void OGRE::initRTSS()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    batb->log << "initRTSS()" << std::endl;
    LogIndent indent( batb->log, "- " );

    // see https://ogrecave.github.io/ogre/api/1.11/rtss.html and Ogre::ApplicationContext::setup()/initialiseRTShaderSystem()
    if ( Ogre::RTShader::ShaderGenerator::initialize() )
    {
        ogre_shader_generator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

        if ( YAML::Node lib = yaml > "RTShaderLib" )
        {
            Ogre::String sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
            
            // always add 'materials' according to ApplicationContext::locateResources().
            if ( true )
            {
                batb->log << "adding resource 'materials'" << std::endl;
                LogIndent indent( batb->log, "- " );

                addResourceGroup( sec, lib > "materials" ); 
            }

            // no ES at this time
            //if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
            //{
            //    batb->log << "adding resource 'glsles'" << std::endl;
            //    LogIndent indent( batb->log, "- " );
            //
            //    addResourceGroup( programs > "glsl" , sec );            
            //    addResourceGroup( programs > "glsles" , sec );            
            //}

            if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl") )
            {
                batb->log << "adding resource 'glsl'" << std::endl;
                LogIndent indent( batb->log, "- " );

                addResourceGroup( sec, lib > "glsl" );            
            }
        }
        else
        {
            throw std::runtime_error( "OGRE: no RTShaderLib path defined" + ogre_rendersystem_name_ );
        }


        // write generated shader to this folder. TODO: better. TODO: may be dynamic data
        auto cache_path = file::directory( file::tmp( "path" ) );
        ogre_shader_generator->setShaderCachePath( cache_path );
        
        // FIXME write cache path

        if ( !ogre_technique_resolver_listener )
        {
            ogre_technique_resolver_listener = new SGTechniqueResolverListener( batb );
            Ogre::MaterialManager::getSingleton().addListener( ogre_technique_resolver_listener );
        }

        // 

        
    }

}
////////////////////////////////////////////////////////////////////////////////
//  OGRE

void OGRE::frameBegin()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    
    if ( init_nonempty() )
    {
        // ensure we are allow to touch Ogre (OGRE is part of non-core BATB)
        if ( enabled_ )
        {
            batb->gl->ogreBegin();

            ////////////////////////////////////////////////////////////////
            // see Root::_renderOneFrame()
            // Root::startRendering() > Root::renderOneFrame 

            bool b;

            // Root::_fireFrameStarted
            b = ogre_root->_fireFrameStarted();
            if ( !b ) batb->log << "WARNING: OGRE: false _fireFrameStarted()" << std::endl;

            //            
            //            
            ////////////////////////////////////////////////////////////////////////////////

            batb->gl->ogreEnd();
        }
    } 

}

void OGRE::frameEnd()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    if ( init_nonempty() )
    {
        // ensure we are allow to touch Ogre (OGRE is part of non-core BATB)
        if ( enabled_ )
        {
            batb->gl->ogreBegin();

            ////////////////////////////////////////////////////////////////////////////////
            // look at definition of OgreRoot::renderOneFrame() !!
            // Root::renderOneFrame() > Root::_updateAllRenderTargets()
            bool b;

            ////////////////////////////////////////////////////////////////
            // see Root::_updateAllRenderTargets()

            // update all targets but don't swap buffers
            ogre_root->getRenderSystem()->_updateAllRenderTargets(false);
            // give client app opportunity to use queued GPU time
            b = ogre_root->_fireFrameRenderingQueued();
            // block for final swap
            ogre_root->getRenderSystem()->_swapAllRenderTargetBuffers();

            // This belongs here, as all render targets must be updated before events are
            // triggered, otherwise targets could be mismatched.  This could produce artifacts,
            // for instance, with shadows.
            SceneManagerEnumerator::Instances::const_iterator it, end = ogre_root->getSceneManagers().end();
            for (it = ogre_root->getSceneManagers().begin(); it != end; ++it)
                it->second->_handleLodEvents();

            if ( !b ) batb->log << "WARNING: OGRE: false _updateAllRenderTargets()" << std::endl;

            
            // Root::_fireFrameEnded
            b = ogre_root->_fireFrameEnded();
            if ( !b ) batb->log << "WARNING: OGRE: false _fireFrameEnded()" << std::endl;

            //            
            //            
            ////////////////////////////////////////////////////////////////////////////////

            batb->gl->ogreEnd();
        }
    }
}


void OGRE::sceneBegin(const Scene& scene)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    // setup ogre
    batb->gl->ogreBegin();

    // tell Ogre the size of our window
    ogre_renderwindow->resize( scene.wth, scene.hth );
  
}

void OGRE::outputCamera(Ogre::Camera* camera)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    // render into previous used viewport of camera
    // (typically just 1 viewport)
    outputViewport( camera->getViewport() );
    
}


void OGRE::outputViewport(Ogre::Viewport* viewport)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    // see Ogre::RenderTarget::_updateViewport()

    if ( viewport )
    {
        Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;

        ogre_rendertarget->_updateViewport( viewport, ogre_statistics_ );
    }

}

void OGRE::sceneEnd()
{


    // reset gl-state after Ogre, for this framd
    batb->gl->ogreEnd();
}


void OGRE::output(const Scene& scene)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );


    if ( init_nonempty() )
    {
        sceneBegin( scene );

        //Ogre::WindowEventUtilities::messagePump(); // TODO: remove this, since we use GLFW! (?)
        
debug::gl::msg( "ogre_root->renderOneFrame();" );
        ogre_root->renderOneFrame();

        sceneEnd();
    }
}

////////////////////////////////////////////////////////////////////////////////
// add resources to Ogre by groups defined in YAML node.
// note that Ogre will not add subdirectories for a resource location.
// 
void OGRE::addResourceGroupsAndInit(const YAML::Node& yaml)
{

    batb->log << "OGRE: adding resources:\n";
    LogIndent indent( batb->log, "* " );

    // iterate over group names
    for (auto i = std::begin( yaml ); i != std::end( yaml ); ++i )
    {
        // NOTE: yaml is a map, not a list. and 'i' is a key. 
        //       this is yaml stuff (scalar, list, map)
        YAML::Node group = i->first;
        std::string name = group.as<std::string>();
        
        batb->log << "'" << name << "':\n";
        LogIndent indent( batb->log, "- " );

        // iterate over defined content for that group
        addResourceGroup( name, i->second );

    }

    try
    {
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }
    catch (std::exception& e)
    {
        batb->log << "OGRE: WARNING: addResourceGroupsAndInit(): " << e.what() << std::endl;
    }

}

void OGRE::addResourceGroup(const std::string& group_name, const YAML::Node& node)
{
    for (auto j = std::begin( node ); j != std::end( node ); ++j )
    {
        YAML::Node resource = *j;
        if ( resource[ "type" ] && resource[ "path" ] )
        {
            std::string type = resource[ "type" ].as<std::string>();
            std::string path = resource[ "path" ].as<std::string>();
            

            // add resource item
            try
            {
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(  file::static_data( path ), type, group_name );
            }
            catch (std::exception& e)
            {
                batb->log << "WARNING: " << e.what();
            }

            batb->log << path;

        }
        else
        {
            batb->log << "WARNING: invalid item definition";
        }
        
        batb->log->endl();
    }
}
void OGRE::enabled(bool e)
{ 
    enabled_ = e; 
}




} // namespace ogre

} // namespace batb

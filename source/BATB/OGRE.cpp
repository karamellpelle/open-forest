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
#include "RenderSystems/GL/include/OgreGLRenderSystem.h"

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
        // see libs/ogre/Samples/Common/src/Cocoa/OgreController.mm
        
        // control the log output from Ogre by creating the LogManager ourselves,
        // before creating Ogre::Root
debug::gl::msg( "OGRE_NEW LogManager" );
        ogre_logmanager = OGRE_NEW Ogre::LogManager();
        auto logpath = file::tmp( "openforest-ogre.log" );
        ogre_logmanager->createLog( logpath, true, false, false );
        // set detail level 
        ogre_logmanager->setLogDetail( Ogre::LL_BOREME );
        batb->log << "Ogre::LogManager writing to " << logpath << std::endl;


        ////////////////////////////////////////////////////////////////////////////////
        // create Ogre ogre_root object
debug::gl::msg( "OGRE_NEW Root" );
        ogre_root = OGRE_NEW Ogre::Root( "", "", "" ); // no files for plugin, config, log
        batb->log << "Ogre::Root created" << std::endl;

        ////////////////////////////////////////////////////////////////////////////////
        // add plugins (ogre_rendersystem, scene managers, ...)
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
                    batb->log << " (ERROR: " << e.what() << " )";
                }

                batb->log->endl();
            }
        }
        else
        {
            batb->log << "ERROR: no 'plugins' defined in .yaml" << std::endl;
            batb->log->indentPop();
            throw std::runtime_error( "OGRE: no 'plugins' defined in config" );
        }

//#if 0
        ////////////////////////////////////////////////////////////////////////////////
        // set ogre_rendersystem for Ogre

        // pick defined RenderSystem, default "OpenGL Rendering Subsystem"
debug::gl::msg( "ogre_root->getRenderSystemByName" );
        ogre_rendersystem_name_ = yaml["ogre_rendersystem"] ? yaml["ogre_rendersystem"].as<std::string>() : "OpenGL Rendering Subsystem";
        ogre_rendersystem = ogre_root->getRenderSystemByName( ogre_rendersystem_name_ );
        

        // set our Ogre render system
        if ( ogre_rendersystem )
        {
debug::gl::msg( "ogre_root->setRenderSystem" );
            ogre_root->setRenderSystem( ogre_rendersystem );
            batb->log << "rendersystem registered: " << ogre_rendersystem_name_ << std::endl;
        }
        else
        {
            batb->log << "ERROR: no RenderSystem with name '" << ogre_rendersystem_name_ << "'" << std::endl;
            batb->log->indentPop();
            throw std::runtime_error( "OGRE: no RenderSystem with name " + ogre_rendersystem_name_ );
        }
//#endif
       
        // xx
	//ogre_root->setRenderSystem(ogre_root->getAvailableRenderers().front());
	ogre_root->getRenderSystem()->setConfigOption("RTT Preferred Mode", "Copy");

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

        // let RenderWindow use our NSWindow. 
        params["externalWindowHandle"] = std::to_string( (unsigned long)( win ) );
        // let RenderWindow use our NSOpenGLContext. 
        params["externalGLContext"] = std::to_string( (unsigned long)( ctx ) );  

#endif
        // OgreRoot::createRenderWindow() -> XXXRenderSystem::_createRenderWindow() -> XXXGLSupport::newWindow() 
        //                                -> XXXWindow::create(). and this creates the XXXContext class too.
        ogre_renderwindow = ogre_root->createRenderWindow( "BATBOgreRenderWindow", 100, 100, false, &params );
        ogre_renderwindow->setVisible(true);
        batb->log << "RenderWindow created" << std::endl;

        ////////////////////////////////////////////////////////////////////////////////
        // Ogre GL context
        // NOTE: must not be setCurrent/set_glfwcontext_ here, since we are loading 
        //       OGRE in another GL context (background thread)
        //glcontextglfw_.begin();

        // emulating OgreRoot::startRendering(), the closed rendering loop of Ogre
        //mActiveRenderer->_initRenderTargets();
        // Clear event times
        ogre_root->clearEventTimes();

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
        init( false );

        
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
        glfwMakeContextCurrent( batb->screen->glfw_window );
    
        //glcontextglfw_.end();

    }
   
}
////////////////////////////////////////////////////////////////////////////////
//  OGRE

void OGRE::frameBegin()
{
    Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;
    
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
    // look at Ogre::Root::startRendering()
    Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;
    
    if ( init_nonempty() )
    {
        if ( enabled_ )
        {
            batb->gl->ogreBegin();

            ////////////////////////////////////////////////////////////////////////////////
            // look at definition of OgreRoot::renderOneFrame() !!
            // Root::renderOneFrame() > Root::_updateAllRenderTargets()
            bool b;

            // void RenderTarget::updateImpl(void)
            //ogre_rendertarget->_endUpdate();

            ////////////////////////////////////////////////////////////////
            // see Root::_updateAllRenderTargets()

            // update all targets but don't swap buffers
            ogre_root->getRenderSystem()->_updateAllRenderTargets(false);
            // give client app opportunity to use queued GPU time
            b = ogre_root->_fireFrameRenderingQueued();
            // block for final swap
            //ogre_root->getRenderSystem()->_swapAllRenderTargetBuffers();

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
    // setup ogre
    batb->gl->ogreBegin();

    // tell Ogre the size of our window
    ogre_renderwindow->resize( scene.wth, scene.hth );
  
}

void OGRE::outputCamera(Ogre::Camera* camera)
{
    Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;
    
    // void RenderTarget::_updateViewport(Viewport* viewport, bool updateStatistics)

    // render into previous used viewport of camera
    // (typically just 1 viewport)
    auto viewport = camera->getViewport();
    if ( viewport )
    {
        assert(viewport->getTarget() == ogre_rendertarget &&
                "RenderTarget::_updateViewport the requested viewport is "
                "not bound to the ogre_rendertarget!");

        //mCamera->_renderScene(this, mShowOverlays);
        bool mShowOverlays = viewport->getOverlaysEnabled();
        camera->_renderScene( viewport, mShowOverlays );
    }
}


void OGRE::outputViewport(Ogre::Viewport* viewport)
{
    Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;
    
    // void RenderTarget::_updateViewport(Viewport* viewport, bool updateStatistics)
    {
        assert(viewport->getTarget() == ogre_rendertarget &&
                "RenderTarget::_updateViewport the requested viewport is "
                "not bound to the ogre_rendertarget!");

        //fireViewportPreUpdate(viewport);
        viewport->update();
        //if(updateStatistics)
        //{
        //    mStats.triangleCount += viewport->_getNumRenderedFaces();
        //    mStats.batchCount += viewport->_getNumRenderedBatches();
        //}
        //fireViewportPostUpdate(viewport);
    }

}

void OGRE::sceneEnd()
{


    // set back our context
    set_glfwcontext_();

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
// 
// NB: only static currently!
void OGRE::addResourceLocation(const YAML::Node& yaml)
{
    // begin GL state for Ogre
    // FIXME: necessary?
    batb->gl->ogreBegin();

    // iterate over group names
    for (auto i = std::begin( yaml ); i != std::end( yaml ); ++i )
    {
        // NOTE: yaml is a map, not a list. and 'i' is a key. 
        //       this is yaml stuff (scalar, list, map)
        YAML::Node group = i->first;
        std::string name = group.as<std::string>();
        
        batb->log << "OGRE: adding items to resource group '" << name << "':\n";

        LogIndent indent( batb->log, "* " );
        // iterate over defined content for that group
        for (auto j = std::begin( i->second ); j != std::end( i->second ); ++j )
        {
            YAML::Node resource = *j;
            if ( resource[ "type" ] && resource[ "path" ] )
            {
                std::string type = resource[ "type" ].as<std::string>();
                std::string path = resource[ "path" ].as<std::string>();
                
                batb->log << path;

                // add resource item
                try
                {
                    //ogre_root->addResourceLocation(  file::static_data( path ), type, name );
	            //Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mResourcePath, std::string("FileSystem"), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
	            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(  file::static_data( path ), type, name );
                }
                catch (Ogre::Exception& e)
                {
                    batb->log << " (" << e.what() << ")";
                }

            }
            else
            {
                batb->log << "(invalid item definition)";
            }
            
            batb->log->endl();
        }
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    batb->gl->ogreEnd();
}

void OGRE::enabled(bool e)
{ 
    enabled_ = e; 
}



void OGRE::set_glfwcontext_()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    // TODO: GL vs GL3Plus
    //
    Ogre::GLRenderSystem* rs = static_cast<Ogre::GLRenderSystem*>( ogre_rendersystem );

    //rs->_switchContext( &glcontextglfw_ );

}

} // namespace ogre

} // namespace batb

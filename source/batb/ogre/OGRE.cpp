//    open-forest: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
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
#include "OgreLogManager.h"
#include "OgreLog.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreCamera.h"
#include "OgreResourceGroupManager.h"
#include "OgreSceneManager.h"
#include "OgreWindowEventUtilities.h"
#include "RenderSystems/GL/include/OgreGLRenderSystem.h"
#include "batb.hpp"
#include "batb/Scene.hpp"


namespace batb
{



namespace ogre
{

using namespace Ogre;
    
////////////////////////////////////////////////////////////////////////////////
//  OGRE

void OGRE::frameBegin()
{
    Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;
    
    if ( init_nonempty() )
    {
        gl::begin_ogre();
        ////////////////////////////////////////////////////////////////////////////////
        // look at definition of OgreRoot::renderOneFrame() !!

        // ( bool Root::renderOneFrame(void) )
        ogre_root->_fireFrameStarted();
        
        // void RenderTarget::updateImpl(void)
        ogre_rendertarget->_beginUpdate();

        ////////////////////////////////////////////////////////////////////////////////
        gl::end_ogre();
    } 

}

void OGRE::frameEnd()
{
    Ogre::RenderTarget* ogre_rendertarget = ogre_renderwindow;
    
    if ( init_nonempty() )
    {
        gl::begin_ogre();
        ////////////////////////////////////////////////////////////////////////////////
        // look at definition of OgreRoot::renderOneFrame() !!

        // void RenderTarget::updateImpl(void)
        ogre_rendertarget->_endUpdate();

        // bool Root::_updateAllRenderTargets(void)
        ogre_root->_fireFrameRenderingQueued();
        ogre_rendersystem->_swapAllRenderTargetBuffers();
        for (SceneManagerEnumerator::SceneManagerIterator it = ogre_root->getSceneManagerIterator(); it.hasMoreElements(); it.moveNext()) 
            it.peekNextValue()->_handleLodEvents();

        // bool Root::renderOneFrame(void)
        ogre_root->_fireFrameEnded();
        ////////////////////////////////////////////////////////////////////////////////
        gl::end_ogre();
    }
}


void OGRE::sceneBegin(const Scene& scene)
{
    // setup ogre
    gl::begin_ogre();

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
    gl::end_ogre();
}


void OGRE::output(const Scene& scene)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );


    if ( init_nonempty() )
    {
        sceneBegin( scene );

        Ogre::WindowEventUtilities::messagePump(); // TODO: remove this, since we use GLFW!
        
debug::gl::msg( "ogre_root->renderOneFrame();" );
        ogre_root->renderOneFrame(); // Q?: do overlays need this, cf. SceneManager::_renderScene?
        //scenemgr->_renderScene( camera, viewport, false ); 

        sceneEnd();
    }
}

////////////////////////////////////////////////////////////////////////////////
// 
void begin(OGRE& ogre)
{

    BATB_LOG_FUNC( ogre.batb );

    if ( ogre.init_empty() )
    {
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );


            ////////////////////////////////////////////////////////////////////////////////
            // setup Ogre
            //

            // control the log output from Ogre by creating the LogManager ourselves,
            // before creating Ogre::Root
debug::gl::msg( "OGRE_NEW LogManager" );
            ogre.ogre_logmanager = OGRE_NEW Ogre::LogManager();
            ogre.ogre_logmanager->createLog( file::tmp( "batb-ogre.log" ), true, false, false );

            ////////////////////////////////////////////////////////////////////////////////
            // create Ogre ogre_root object
debug::gl::msg( "OGRE_NEW Root" );
            ogre.ogre_root = OGRE_NEW Ogre::Root( "", "", "" ); // no files for plugin, config, log
           
            ////////////////////////////////////////////////////////////////////////////////
            // add plugins (ogre_rendersystem, scene managers, ...)
            ogre.batb.log << "OGRE: loading plugins:" << std::endl;
            if ( YAML::Node plugins = ogre.yaml[ "plugins" ] )
            {
                for (auto i = std::begin( plugins ); i != std::end( plugins ); ++i )
                {
                    std::string plugin = i->as<std::string>();
                    ogre.batb.log << "  " << plugin;
                    
                    try
                    {
std::ostringstream os; os << "ogre.ogre_root->loadPlugin( " << plugin << ")";                
debug::gl::msg( os.str() );
                        ogre.ogre_root->loadPlugin( plugin );
                    }
                    catch (Ogre::Exception& e)
                    {
                        ogre.batb.log << " (" << e.what() << " )";
                    }

                    ogre.batb.log << std::endl;
                }
            }
            else
            {
                throw std::runtime_error( "OGRE: no 'plugins' defined in config" );
            }

            ////////////////////////////////////////////////////////////////////////////////
            // set ogre_rendersystem for Ogre

            // pick defined RenderSystem, default "OpenGL Rendering Subsystem"
debug::gl::msg( "ogre.ogre_root->getRenderSystemByName" );
            ogre.ogre_rendersystem_name_ = ogre.yaml["ogre_rendersystem"] ? ogre.yaml["ogre_rendersystem"].as<std::string>() : "OpenGL Rendering Subsystem";
            ogre.ogre_rendersystem = ogre.ogre_root->getRenderSystemByName( ogre.ogre_rendersystem_name_ );

            // set our Ogre render system
            if ( ogre.ogre_rendersystem )
            {
debug::gl::msg( "ogre.ogre_root->setRenderSystem" );
                ogre.ogre_root->setRenderSystem( ogre.ogre_rendersystem );
            }
            else
            {
                throw std::runtime_error( "OGRE: no RenderSystem with name " + ogre.ogre_rendersystem_name_ );
            }

            ////////////////////////////////////////////////////////////////////////////////    
            // initialize Root, using our defined RenderSystem
            // (this method returns nullptr, since our argument is 'false')
debug::gl::msg( "ogre.ogre_root->initialise" );
            ogre.ogre_root->initialise( false ); 


            ////////////////////////////////////////////////////////////////////////////////
            // create an Ogre window, using our existing GLFW window.
            //
            // see implementation of Ogre::GLXWindow for settings:
            //  - currentGLContext 
            //  - FSAA
            //  - ... 
debug::gl::msg( "ogre.ogre_root->createRenderWindow()" );
            Ogre::NameValuePairList params;
            params["currentGLContext"] = "true";  // let RenderWindow use our GL context
            params["externalGLControl"] = "true"; // no automatic swapping
            ogre.ogre_renderwindow = ogre.ogre_root->createRenderWindow( "GLFWRenderWindow", 0, 0, false, &params );
            ogre.ogre_renderwindow->setVisible(true);

            ////////////////////////////////////////////////////////////////////////////////
            // Ogre GL context
            // NOTE: must not be setCurrent/set_glfwcontext_ here, since we are loading 
            //       OGRE in another GL context (background thread)
            begin( ogre.glcontextglfw_ );



    }
    ////////////////////////////////////////////////////////////////////////////////
    
    ogre.init( true );


    
}


void end(OGRE& ogre)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );
    BATB_LOG_FUNC( ogre.batb );

    if ( ogre.init_nonempty() )
    {
        ogre.save();
   
        // needs to be done before modding Ogre, since
        // frameBegin/End may be done in other thread (i.e. main thread)
        ogre.init( false );

        
debug::gl::msg( "OGRE_DELETE Root" );
        OGRE_DELETE ogre.ogre_root;
debug::gl::msg( "OGRE_DELETE LogManager" );
        OGRE_DELETE ogre.ogre_logmanager;

        ogre.ogre_root = nullptr;

        // since Ogre steal our GLXContext we have to rebind it after 
        // Ogre shutdown
        ogre.glcontextglfw_.setCurrent();
    
        end( ogre.glcontextglfw_ );

    }
   
}

void OGRE::set_glfwcontext_()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );
    // TODO: GL vs GL3Plus
    //
    Ogre::GLRenderSystem* rs = static_cast<Ogre::GLRenderSystem*>( ogre_rendersystem );

    rs->_switchContext( &glcontextglfw_ );

}

} // namespace ogre

} // namespace batb

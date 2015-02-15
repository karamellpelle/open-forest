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
#include "batb.hpp"
#include "batb/Scene.hpp"
#include "OgreLogManager.h"
#include "OgreLog.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreResourceGroupManager.h"
#include "OgreSceneManager.h"
#include "OgreWindowEventUtilities.h"
#include "RenderSystems/GL/include/OgreGLRenderSystem.h"


namespace batb
{



namespace ogre
{


////////////////////////////////////////////////////////////////////////////////
//  OGRE

OGRE::OGRE(BATB& b) : batb( b )
{

}



void OGRE::save()
{

    // FIXME: write to file
}


void OGRE::output(const Scene& scene)
{
debug::gl_push_group( DEBUG_FUNCTION_NAME );

    if ( initialized_ )
    {
        // tell Ogre the size of our window
        renderwindow->resize( scene.wth, scene.hth );

        Ogre::WindowEventUtilities::messagePump(); // TODO: remove this, since we use GLFW!
        
debug::gl( "root->renderOneFrame();" );
        root->renderOneFrame();
        //scenemgr->_renderScene( camera, viewport, false );

        // set back our context
debug::gl( "set_glfwcontext_();" );
        set_glfwcontext_();

        
    }
debug::gl_pop_group();
}

////////////////////////////////////////////////////////////////////////////////
// 
void begin(OGRE& ogre)
{

    BATB_LOG_FUNC( ogre.batb );

debug::gl_push_group( DEBUG_FUNCTION_NAME );

    // set up this OGRE object from file
    YAML::Node yaml = YAML::LoadFile( ogre.filepath_ );

    ////////////////////////////////////////////////////////////////////////////////
    // setup Ogre
    //

    // control the log output from Ogre by creating the LogManager ourselves,
    // before creating Ogre::Root
debug::gl( "OGRE_NEW LogManager" );
    ogre.logmanager = OGRE_NEW Ogre::LogManager();
    ogre.logmanager->createLog( file::tmp( "batb-ogre.log" ), true, false, false );

    ////////////////////////////////////////////////////////////////////////////////
    // create Ogre root object
debug::gl( "OGRE_NEW Root" );
    ogre.root = OGRE_NEW Ogre::Root( "", "", "" ); // no files for plugin, config, log
   
    ////////////////////////////////////////////////////////////////////////////////
    // add plugins (rendersystem, scene managers, ...)
    ogre.batb.log << "OGRE: loading plugins:" << std::endl;
    if ( YAML::Node plugins = yaml[ "plugins" ] )
    {
        for (auto i = std::begin( plugins ); i != std::end( plugins ); ++i )
        {
            std::string plugin = i->as<std::string>();
            ogre.batb.log << "  " << plugin;
            
            try
            {
                
debug::gl( "ogre.root->loadPlugin" );
                ogre.root->loadPlugin( plugin );
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
    // set rendersystem for Ogre

    // pick defined RenderSystem, default "OpenGL Rendering Subsystem"
debug::gl( "ogre.root->getRenderSystemByName" );
    ogre.rendersystem_name_ = yaml["rendersystem"] ? yaml["rendersystem"].as<std::string>() : "OpenGL Rendering Subsystem";
    ogre.rendersystem = ogre.root->getRenderSystemByName( ogre.rendersystem_name_ );

    // set our Ogre render system
    if ( ogre.rendersystem )
    {
debug::gl( "ogre.root->setRenderSystem" );
        ogre.root->setRenderSystem( ogre.rendersystem );
    }
    else
    {
        throw std::runtime_error( "OGRE: no RenderSystem with name " + ogre.rendersystem_name_ );
    }

    ////////////////////////////////////////////////////////////////////////////////    
    // initialize Root, using our defined RenderSystem
    // (this method returns nullptr, since our argument is 'false')
debug::gl( "ogre.root->initialise" );
    ogre.root->initialise( false ); 


    ////////////////////////////////////////////////////////////////////////////////
    // 
debug::gl( "begin( ogre.glcontextglfw_ )" );
    begin( ogre.glcontextglfw_ );

    ////////////////////////////////////////////////////////////////////////////////
    // create an Ogre window, using our existing GLFW window.
    //
    // see implementation of Ogre::GLXWindow for settings:
    //  - currentGLContext 
    //  - FSAA
    //  - ... 
debug::gl( "ogre.root->createRenderWindow()" );
    Ogre::NameValuePairList params;
    params["currentGLContext"] = "true";  // let RenderWindow use our GL context
    params["externalGLControl"] = "true"; // no automatic swapping
    ogre.renderwindow = ogre.root->createRenderWindow( "GLFWRenderWindow", 0, 0, false, &params );
    ogre.renderwindow->setVisible(true);

    // set back our context
debug::gl( "ogre.set_glfwcontext_()" );
    ogre.set_glfwcontext_();




    ////////////////////////////////////////////////////////////////////////////////
    
    ogre.initialized_ = true;


    
debug::gl_pop_group();
}


void end(OGRE& ogre)
{
    BATB_LOG_FUNC( ogre.batb );
debug::gl_push_group( DEBUG_FUNCTION_NAME );

    if ( ogre.initialized_ )
    {
        ogre.save();

        
debug::gl( "OGRE_DELETE Root" );
        OGRE_DELETE ogre.root;
debug::gl( "OGRE_DELETE LogManager" );
        OGRE_DELETE ogre.logmanager;

        ogre.root = nullptr;

        // since Ogre steal our GLXContext we have to rebind it after 
        // Ogre shutdown
debug::gl( " ogre.glcontextglfw_.setCurrent()" );
        ogre.glcontextglfw_.setCurrent();
    
debug::gl( "end( ogre.glcontextglfw_ )" );
        end( ogre.glcontextglfw_ );

    }
   
debug::gl_pop_group();
    ogre.initialized_ = false;

}

void OGRE::set_glfwcontext_()
{
    // TODO: GL vs GL3Plus
    //
    Ogre::GLRenderSystem* rs = static_cast<Ogre::GLRenderSystem*>( rendersystem );

    rs->_switchContext( &glcontextglfw_ );

}

} // namespace ogre

} // namespace batb

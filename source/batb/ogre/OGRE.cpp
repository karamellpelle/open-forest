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
    if ( initialized_ )
    {
        // tell Ogre the size of our window
        renderwindow->resize( scene.wth, scene.hth );

        Ogre::WindowEventUtilities::messagePump();

        root->renderOneFrame();
        //scenemgr->_renderScene( camera, viewport, false );

        // TODO
        // set back our context
        //rendersystem->_switchContext( &batbcontext_ );
        
    }

}

////////////////////////////////////////////////////////////////////////////////
// 
void begin(OGRE& ogre)
{

    BATB_LOG_FUNC( ogre.batb );

    // set up this OGRE object from file
    YAML::Node yaml = YAML::LoadFile( ogre.filepath_ );

    ////////////////////////////////////////////////////////////////////////////////
    // setup Ogre
    //

    // control the log output from Ogre by creating the LogManager ourselves,
    // before creating Ogre::Root
    ogre.logmanager = OGRE_NEW Ogre::LogManager();
    ogre.logmanager->createLog( file::tmp( "batb-ogre.log" ), true, false, false );

    ////////////////////////////////////////////////////////////////////////////////
    // create Ogre root object
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
    std::string rendersystem_name = yaml["rendersystem"] ? yaml["rendersystem"].as<std::string>() : "OpenGL Rendering Subsystem";
    Ogre::RenderSystem* renderer = ogre.root->getRenderSystemByName( rendersystem_name );

    // set our Ogre render system
    if ( renderer )
    {
        ogre.root->setRenderSystem( renderer );
    }
    else
    {
        throw std::runtime_error( "OGRE: no RenderSystem with name " + rendersystem_name );
    }

    ////////////////////////////////////////////////////////////////////////////////    
    // initialize Root, using our defined RenderSystem
    // (this method returns nullptr, since our argument is 'false')
    ogre.root->initialise( false ); 


    ////////////////////////////////////////////////////////////////////////////////
    // 
    begin( batbcontext_ );

    ////////////////////////////////////////////////////////////////////////////////
    // create an Ogre window, using our existing GLFW window.
    //
    // see implementation of Ogre::GLXWindow for settings:
    //  - currentGLContext 
    //  - FSAA
    //  - ... 
    Ogre::NameValuePairList params;
    params["currentGLContext"] = "true";  // let RenderWindow use our GL context
    params["externalGLControl"] = "true"; // no automatic swapping
    ogre.renderwindow = ogre.root->createRenderWindow( "GLFWRenderWindow", 0, 0, false, &params );
    ogre.renderwindow->setVisible(true);

    // TODO
    // set back our context
    //rendersystem->_switchContext( &batbcontext_ );



    ////////////////////////////////////////////////////////////////////////////////
    
    ogre.initialized_ = true;


    // (now create scene manager!)
}


void end(OGRE& ogre)
{
    ogre.batb.log << THIS_FUNCTION << std::endl;    

    if ( ogre.initialized_ )
    {
        ogre.save();

        OGRE_DELETE ogre.root;
        OGRE_DELETE ogre.logmanager;

        ogre.root = nullptr;

        // since Ogre steal our GLXContext we have to rebind it after 
        // Ogre shutdown
        ogre.batbcontext_.setCurrent();
    
        end( batbcontext_ );

    }
   
    ogre.initialized_ = false;

}


} // namespace ogre

} // namespace batb

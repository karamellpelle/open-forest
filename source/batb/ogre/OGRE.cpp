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
#include "OgreLogManager.h"
#include "OgreLog.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreResourceGroupManager.h"




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



    // TODO: 
    //  - remove frm here, into other, for example tmp::ogre::demo_begin
    //  - find out how to use ResourceGroupManager::initialiseAllResourceGroups()
    ////////////////////////////////////////////////////////////////////////////////
    // define our resources
    if ( YAML::Node resources = yaml[ "resources" ] )
    {
        // iterate over group names
        for (auto i = std::begin( resources ); i != std::end( resources ); ++i )
        {
            // NOTE: resources is a map, not a list. and 'i' is a key. 
            //       this is yaml stuff (scalar, list, map)
            YAML::Node group = i->first;
            std::string name = group.as<std::string>();
            
            ogre.batb.log << "OGRE: adding items to resource group '" << name << "':\n";
            // iterate over defined content for that group
            for (auto j = std::begin( i->second ); j != std::end( i->second ); ++j )
            {
                ogre.batb.log << "  ";

                YAML::Node resource = *j;
                if ( resource[ "type" ] && resource[ "path" ] )
                {
                    std::string type = resource[ "type" ].as<std::string>();
                    std::string path = resource[ "path" ].as<std::string>();
                    
                    ogre.batb.log << path;

                    // add resource item
                    try
                    {
                        ogre.root->addResourceLocation(  file::static_data( path ), type, name );
                    }
                    catch (Ogre::Exception& e)
                    {
                        ogre.batb.log << " (" << e.what() << ")";
                    }

                }
                else
                {
                    ogre.batb.log << "(invalid item definition)";
                }

                ogre.batb.log << "\n";
            }
        }
    }
    else
    {
        throw std::runtime_error( "OGRE: no 'resources' defined in config" );
    }
    // TODO: find correct usage for this:
    //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


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

        // FIXME: does this free all resources (unloadPlugin, ...)?
        OGRE_DELETE ogre.root;
        OGRE_DELETE ogre.logmanager;

        ogre.root = nullptr;
    }
    
    ogre.initialized_ = false;

}


} // namespace ogre

} // namespace batb

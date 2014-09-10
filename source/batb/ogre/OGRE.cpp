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
#include "OgreRoot.h"
#include "OgreRenderWindow.h"




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

    ogre.batb.log << THIS_FUNCTION << std::endl;    

    // set up this OGRE object from file
    YAML::Node yaml = YAML::LoadFile( ogre.filepath_ );

    // using Ogre:
    //  - plugins has to be registered (RenderSystem's, ...)
    //  - resources has to be defined

    ////////////////////////////////////////////////////////////////////////////////
    // TODO: OgreLogManager: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Basic+Ogre+Framework&structure=Tutorials


    ////////////////////////////////////////////////////////////////////////////////
    // create Ogre root object

    ogre.root_ = new Ogre::Root( "", "", file::tmp( "batb-ogre.log" ) ); // no files for plugin, config - we set these programatically!
    
    // add plugins (rendersystem, scene managers, ...)
    if ( YAML::Node plugins = yaml[ "plugins" ] )
    {
        for (auto i = std::begin( plugins ); i != std::end( plugins ); ++i )
        {
            std::string plugin = i->as<std::string>();
            ogre.batb.log << "OGRE: loading plugin '" << plugin << "'... ";
            
            try
            {
                ogre.root_->loadPlugin( plugin );
                ogre.batb.log << "OK.";
            }
            catch (Ogre::Exception& e)
            {
                ogre.batb.log << "error: " << e.what();
            }

            ogre.batb.log << std::endl;
        }
    }
    else
    {
        throw std::runtime_error( "OGRE: no 'plugins' defined in config" );
    }


    ////////////////////////////////////////////////////////////////////////////////
    // define our resources
    Ogre::ResourceGroupManager* resgrpmgr = Ogre::ResourceGroupManager::getSingletonPtr();
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
                YAML::Node resource = *j;
                if ( resource[ "type" ] && resource[ "path" ] )
                {
                    std::string type = resource[ "type" ].as<std::string>();
                    std::string path = resource[ "path" ].as<std::string>();
                    
                    ogre.batb.log << "        " << path << "\n";

                    // add resource item
                    try
                    {
                        resgrpmgr->addResourceLocation( file::static_data( path ), type, name );
                    }
                    catch (Ogre::Exception& e)
                    {
                        ogre.batb.log << "        (" << e.what() << ")\n";
                    }
                }
                else
                {
                    ogre.batb.log << "        (invalid item definition)\n";
                }
            }
        }
    }
    else
    {
        throw std::runtime_error( "OGRE: no 'resources' defined in config" );
    }

    ////////////////////////////////////////////////////////////////////////////////
    // set rendersystem for Ogre

    Ogre::RenderSystem* renderer = nullptr;

    // we pick the 0th of available, which should be correct!
    Ogre::RenderSystemList renderers = ogre.root_->getAvailableRenderers();
    renderer = renderers.empty() ? nullptr : renderers[0];
    // FIXME: use getRenderSystemByName( "OpenGL ..." );!

    // define our render system for Ogre
    if ( renderer )
    {
        ogre.root_->setRenderSystem( renderer );
    }
    else
    {
        throw std::runtime_error( "OGRE: no renderers available." );
    }

    // now initialize Ogre::Root, using our defined RenderSystem
    // (this method returns nullptr, since our argument is 'false')
    ogre.root_->initialise( false ); 


    ////////////////////////////////////////////////////////////////////////////////
    // create an Ogre window, from our GLFW window:

    Ogre::NameValuePairList params;

    // let the renderwindow use our GLContext:
    params["currentGLContext"] = "true";                        
#ifdef GLFW_EXPOSE_NATIVE_GLX
    // let Ogre use our GLContext:
    GLXContext context = glfwGetGLXContext( env::screen_window() );
    unsigned long value = (unsigned long)( context );
    params["externalGLContext"] = value;
#endif
    ogre.renderwindow_ = ogre.root_->createRenderWindow( "__assert_no_window_name__", 0, 0, false, &params );
    ogre.renderwindow_->setVisible(true);


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
        delete ogre.root_;
        ogre.root_ = nullptr;
    }
    
    ogre.initialized_ = false;

}


} // namespace ogre

} // namespace batb

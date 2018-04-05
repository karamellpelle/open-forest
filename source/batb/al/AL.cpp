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
#include "batb.hpp"
#include "batb/Scene.hpp"
#include <algorithm>


namespace batb
{



namespace al
{


////////////////////////////////////////////////////////////////////////////////
//  AL


/*
void AL::output(const Scene& scene)
{
}
*/


void AL::frameBegin()
{
    if ( init_nonempty() )
    { 
        if ( enabled_ )
        {

        }
    }

}

void AL::frameEnd()
{
    if ( init_nonempty() )
    { 
        if ( enabled_ )
        {

            // try to release buffers (if no source is using it)
            auto i = std::begin( buffers_ );
            while ( i != std::end( buffers_ ) )
            {
                alure::Buffer buf = *i;

                // release sources that are not using this buffer anymore
                auto srcs = buf.getSources();

                if ( srcs.empty() )
                {
                    al_context.removeBuffer( buf );
                    i = buffers_.erase( i );
                }
                else
                {
                    // remove non-playing sources. this should not be done
                    // since a source could be used multiple times
                    //for ( auto src : srcs )
                    //{
                    //    if ( !src.isPlaying() )
                    //    {
                    //        src.destroy();
                    //    }
                    //}

                    ++i;
                }
               
            }

            // update ALURE/AL
            al_context.update();
        }
    }

}


alure::Source AL::source(const std::string& path, const glm::mat4& mat)
{
    // "multiple calls with the same name will return the same buffer"
    //    - alure doc
    auto buf = al_context.getBuffer( path );
    if ( std::find( std::begin( buffers_ ), std::end( buffers_ ), buf ) == std::end( buffers_ ) )
    {
        buffers_.push_back( buf );
    }
   
    auto src = al_context.createSource();

    auto pos = mat[3];
    src.setPosition( { pos.x, pos.y, pos.z } );
    auto at = mat[2]; // z
    auto up = mat[1]; // y
    src.setOrientation( { { at.x, at.y, at.z } , { up.x, up.y, up.z } } );

    // TODO: dont play.
    // and play
    src.play( buf );

    return src;
}

////////////////////////////////////////////////////////////////////////////////
// 
void begin(AL& al)
{

    BATB_LOG_FUNC( al.batb );
    
    if ( al.init_empty() )
    {

        try
        {
            //////////////////////////////////////////////////
            al.al_devmgr = alure::DeviceManager::getInstance();

            // FIXME: memory leak, according to valgrind
            al.al_device = al.al_devmgr.openPlayback(); 

            al.batb.log << "AL: opened al_device \"" << al.al_device.getName() << "\"" << std::endl;

            al.al_context = al.al_device.createContext();
            alure::Context::MakeCurrent( al.al_context );

            // there is only one listener
            al.al_listener = al.al_context.getListener();

            //////////////////////////////////////////////////////////
            //      OpenAL
        }
        catch (std::exception& e)
        {
            al.batb.log << "could not init AL: " << e.what() << std::endl;
            throw e;
        }



    }
    ////////////////////////////////////////////////////////////////////////////////
    
    al.init( true );



    
}


void end(AL& al)
{
    BATB_LOG_FUNC( al.batb );

    if ( al.init_nonempty() )
    {
        al.save();

        // Buffer's must be destroyed according to doc
        for ( auto buf : al.buffers_ )
        {
            al.al_context.removeBuffer( buf );
        }

        alure::Context::MakeCurrent( nullptr );
        al.al_context.destroy();
        al.al_device.close();
        
    }
   
    al.init( false );
}

} // namespace al

} // namespace batb

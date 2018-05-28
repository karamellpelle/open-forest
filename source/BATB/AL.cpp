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
#include "BATB/AL.hpp"
#include "BATB/Scene.hpp"


namespace batb
{



namespace al
{


////////////////////////////////////////////////////////////////////////////////
// setup

void AL::begin(const std::string& path)
{
    batb->log << "batb->al->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
        
        // set configuration file
        config( path );

        try
        {
            //////////////////////////////////////////////////
            al_devmgr = alure::DeviceManager::getInstance();

            // FIXME: memory leak, according to valgrind
            al_device = al_devmgr.openPlayback(); 

            batb->log << "opened al_device '" << al_device.getName() << "'" << std::endl;

            al_context = al_device.createContext();
            alure::Context::MakeCurrent( al_context );
            batb->log << "context created" << std::endl;

            // there is only one listener
            al_listener = al_context.getListener();

            //////////////////////////////////////////////////////////
            //      OpenAL
        }
        catch (std::exception& e)
        {
            batb->log << "ERROR: could not initialize (" << e.what() << ")" << std::endl;
            batb->log->indentPop();
            throw e;
        }

        // enable functionality should not be necessary, since AL is part of BATB core
        enabled( true );

    }
    ////////////////////////////////////////////////////////////////////////////////
    
    init( true );



    
}


void AL::end()
{

    batb->log << "batb->al->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        save();

        // Buffer's must be destroyed according to doc
        for ( auto buf : buffers_ )
        {
            al_context.removeBuffer( buf );
        }
        batb->log << "buffers removed" << std::endl;

        alure::Context::MakeCurrent( nullptr );
        al_context.destroy();
        batb->log << "context destroyed" << std::endl;
        al_device.close();
        batb->log << "device closed" << std::endl;
        
    }
   
    init( false );
}


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


} // namespace al

} // namespace batb

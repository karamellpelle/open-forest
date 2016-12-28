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
            al_context->update();

            // try to release buffers (if no source is using it)
            auto i = std::begin( buffers_ );
            while ( i != std::end( buffers_ ) )
            {
                alure::Buffer* buf = *i;

                // release sources that are not using this buffer anymore
                auto srcs = buf->getSources();
                for ( auto src : srcs )
                {
                    if ( !src->isPlaying() )
                    {
                        src->release();
                    }
                }
               
                // if no Source is using the buffer, remove it from context and 'buffers_'
                if ( buf->isInUse() )
                {
                    ++i;
                }
                else
                {
                    al_context->removeBuffer( buf );
                    i = buffers_.erase( i );
                }
            }
        }
    }

}


alure::Source* AL::source(const std::string& path, const glm::mat4& mat)
{
    // "multiple calls with the same name will return the same buffer"
    //    - alure doc
    auto* buf = al_context->getBuffer( path );
    if ( std::find( std::begin( buffers_ ), std::end( buffers_ ), buf ) == std::end( buffers_ ) )
    {
        buffers_.push_back( buf );
    }
   
    auto* src = al_context->getSource();
    //auto* src = al_context->createSource();

    auto pos = mat[3];
    src->setPosition( pos.x, pos.y, pos.z );
    auto at = mat[2]; // z
    auto up = mat[1]; // y
    src->setOrientation( at.x, at.y, at.z, up.x, up.y, up.z );

    // and play
    src->play( buf );

    return src;
}

////////////////////////////////////////////////////////////////////////////////
// 
void begin(AL& al)
{

    BATB_LOG_FUNC( al.batb );
    
    if ( al.init_empty() )
    {
        //////////////////////////////////////////////////
        // TODO: check nullptr!
        alure::DeviceManager& devmgr = alure::DeviceManager::get();

        // FIXME: memory leak, according to valgrind
        al.al_device = devmgr.openPlayback(); 

        al.batb.log << "AL: opened al_device \"" << al.al_device->getName() << "\"" << std::endl;

        al.al_context = al.al_device->createContext();
        alure::Context::MakeCurrent( al.al_context );


        //////////////////////////////////////////////////////////
        //      OpenAL



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

        alure::Context::MakeCurrent( nullptr );
        al.al_context->destroy();
        al.al_context = nullptr;
        al.al_device->close();
        al.al_device = nullptr;
        
    }
   
    al.init( false );
}

} // namespace al

} // namespace batb

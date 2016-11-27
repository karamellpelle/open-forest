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
            alure_context->update();
        }
    }

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
        al.alure_device = devmgr.openPlayback(); 

        al.batb.log << "AL: opened alure_device \"" << al.alure_device->getName() << "\"" << std::endl;

        al.alure_context = al.alure_device->createContext();
        alure::Context::MakeCurrent( al.alure_context );


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
        al.alure_context->destroy();
        al.alure_context = nullptr;
        al.alure_device->close();
        al.alure_device = nullptr;
        
    }
   
    al.init( false );
}

} // namespace al

} // namespace batb

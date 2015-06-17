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

AL::AL(BATB& b) : batb( b )
{

}



void AL::save()
{

    // FIXME: write to file
}

/*
void AL::output(const Scene& scene)
{
}
*/

////////////////////////////////////////////////////////////////////////////////
// 
void begin(AL& al)
{

    BATB_LOG_FUNC( al.batb );

    // set up this AL object from file
    YAML::Node yaml = YAML::LoadFile( al.filepath_ );

    //////////////////////////////////////////////////
    // TODO: check nullptr!

    al.devicemanager = alure::DeviceManager::get();

    al.device = al.devicemanager->openPlayback();

    al.batb.log << "AL: opened device \""
                << al.device->getName( alure::PlaybackDevType_Basic )
                << "\"" << std::endl;

    al.context = al.device->createContext();
    alure::Context::MakeCurrent( al.context );

    ////////////////////////////////////////////////////////////////////////////////
    
    al.initialized_ = true;


    
}


void end(AL& al)
{
    BATB_LOG_FUNC( al.batb );

    if ( al.initialized_ )
    {
        al.save();

        alure::Context::MakeCurrent( nullptr );
        al.context->destroy();
        al.context = nullptr;
        al.device->close();
        al.device = nullptr;
        
    }
   
    al.initialized_ = false;

}

} // namespace al

} // namespace batb

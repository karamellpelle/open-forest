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
            alure_devmgr = alure::DeviceManager::getInstance();

            // FIXME: memory leak, according to valgrind
            alure_device = alure_devmgr.openPlayback(); 

            batb->log << "opened alure_device '" << alure_device.getName() << "'" << std::endl;

            alure_context = alure_device.createContext();
            alure::Context::MakeCurrent( alure_context );
            batb->log << "context created" << std::endl;

            // there is only one listener
            alure_listener = alure_context.getListener();

            // set distance model
            // AL_INVERSE_DISTANCE_CLAMPED makes it possible to use the settings 
            // AL_REFERENCE_DISTANCE, AL_MAX_DISTANCE and AL_ROLLOFF_FACTOR
            //alure_context.setDistanceModel( alure::DistanceModel::InverseClamped ); // can't get this working well :(
            //alure_context.setDistanceModel( alure::DistanceModel::LinearClamped );
            alure_context.setDistanceModel( alure::DistanceModel::Exponent );

        }
        catch (std::exception& e)
        {
            batb->log << "ERROR: could not initialize (" << e.what() << ")" << std::endl;
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

        alure::Context::MakeCurrent( nullptr );
        alure_context.destroy();
        batb->log << "context destroyed" << std::endl;
        alure_device.close();
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

            // update ALURE/AL
            alure_context.update();
        }
    }

}


alure::Source AL::play2D(const std::string& path)
{
    // "multiple calls with the same name will return the same buffer"
    //    - alure doc
    auto buf = alure_context.getBuffer( path );
    auto src = alure_context.createSource();

    // TODO: make sure src is 2D

    // play
    src.play( buf );

    return src;
}


} // namespace al

} // namespace batb

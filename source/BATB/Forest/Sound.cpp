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
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Sound.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Value/Forest.hpp"
#include "BATB/Run/World.hpp"

#define KEEP_CAMERA_ABOVE_GROUND

namespace batb
{

namespace forest
{

Sound::Sound() 
{

}

void Sound::init(World* forest)
{
    // since we work directly with references to World, we can't initialize again.
    if ( forest_ ) throw std::runtime_error( "init() non-empty Sound" );


    // always work on this World
    forest_ = forest;

    
}

void Sound::play3D(const Aim& aim)
{
    // also set listener to camera Aim
    // FIXME: correct camera Aim matrix (roll not used)
    auto pos = aim.aim[3];
    auto lookat = aim.aim[2];
    auto up = aim.aim[1];
    alure_listener.setPosition( { pos.x, pos.y, pos.z } );
    alure_listener.setOrientation( {{lookat.x, lookat.y, lookat.z}, {up.x, up.y, up.z}} );

}

void Sound::play2D()
{
    
}

void Sound::step(tick_t tick)
{
    // make sure we can handle very fast framerate (tick == tick_)
    while ( tick_ + value::dt <= tick )
    {

        tick_ += value::dt;
    }
}


} // namespace forest

} // namespace batb




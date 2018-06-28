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
#ifndef BATB_AL_HPP
#define BATB_AL_HPP
#include "BATB/ModuleBATB.hpp"
#include "AL/alure2.h"



namespace batb
{

class Scene;


namespace al
{




class AL : public ModuleBATB
{
public:
    AL(BATB* b) : ModuleBATB( b ) { }

    // setup
    void begin(const std::string& );
    void end();


    // should AL be ignored each frame?
    void enabled(bool e) { enabled_ = e; }

    // render to Scene
    //void output(const Scene& );
    void frameBegin();
    void frameEnd();


    // play in background (music for example)
    alure::Source play2D(const std::string& );


    alure::DeviceManager alure_devmgr;
    alure::Device        alure_device;
    alure::Context       alure_context;
    alure::Listener      alure_listener;
  

private:
    bool enabled_ = false;
   
    // for playing sources
    std::list<alure::Buffer> buffers_;
};


} // namespace al

} // namespace batb

#endif


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

    // create and play a source. the Buffer is created automatically.
    // the source must be removed with Source::destroy() when not needed anymore.
    // its corresponing buffer will then be removed (by AL::frameEnd())
    //
    // TODO: this should be done better. don't play on creation. let objects take 
    // care of the source including destroying it.
    alure::Source source(const std::string& , const glm::mat4& );
    //alure::Source& source(const std::string& ); // <- TODO: no matrix means background sound


    alure::DeviceManager al_devmgr;
    alure::Device        al_device;
    alure::Context       al_context;
    alure::Listener      al_listener;
  

private:
    bool enabled_ = false;
   
    // for playing sources
    std::list<alure::Buffer> buffers_;
};


} // namespace al

} // namespace batb

#endif


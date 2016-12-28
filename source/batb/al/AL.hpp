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
#ifndef BATB_AL_AL_HPP
#define BATB_AL_AL_HPP
#include "batb/batb_include.hpp"
#include "batb/ModuleBATB.hpp"
#include "AL/alure2.h"



namespace batb
{

class Scene;


namespace al
{




class AL : public ModuleBATB
{
friend void begin(AL& );
friend void end(AL& );

public:
    AL(BATB& b) : ModuleBATB( b ) { }

    // should AL be ignored each frame?
    void enabled(bool e) { enabled_ = e; }

    // render to Scene
    //void output(const Scene& );
    void frameBegin();
    void frameEnd();

    // create and play a source. the returned object is only allowed to be used
    // instantly to setup the source, because the created source will automatically
    // be released (i.e. destroyed) by 'AL' at end of some frame when the source is
    // finished.
    alure::Source* source(const std::string& , const glm::mat4& );
    //alure::Source& source(const std::string& ); // <- TODO: no matrix means background sound

    alure::Device* al_device = nullptr;
    alure::Context* al_context = nullptr;


private:
    bool enabled_ = true;
   
    // for playing sources
    std::list<alure::Buffer*> buffers_;
};


void begin(AL& );
void end(AL& );

} // namespace al

} // namespace batb

#endif


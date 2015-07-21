//    open-forest: an orienteering game.
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
#ifndef ENV_ENV_HPP
#define ENV_ENV_HPP
#include "env/env_include.hpp"
#include "Module.hpp"


namespace env
{

class Env : public Module
{
friend void begin(Env& );
friend void end(Env& );

public:
    Env() = default;

    GLFWwindow* window = nullptr;
  
    ////////////////////////////////////////////////////////////////
    // frame
    void frameBegin();
    void frameEnd();
    uint frameCount()                               { return frame_count_; }
    float_t frameFPS()                              { return frame_fps_; }

    ////////////////////////////////////////////////////////////////
    // screen
    void screenSize(uint& wth, uint& hth);
    void screenShape(float_t& wth, float_t& hth);
    void screenSetFBO0()                            { screenFBO( fbo0_ ); }
    void screenSetFBO1()                            { screenFBO( fbo1_ ); }
    GLuint screenFBO()                              { return fbo_; }
    GLuint screenFBO(GLuint );
    
    ////////////////////////////////////////////////////////////////
    // tick
    tick_t tick() const                             { return tick_; }           // tick for frame
    tick_t tickNow() const                          { return glfwGetTime(); }   // current tick

    ////////////////////////////////////////////////////////////////
    // sound

    ////////////////////////////////////////////////////////////////
    // ...

private:
    tick_t tick_ = 0.0;

    GLuint fbo_ = 0;
    GLuint fbo0_ = 0;
    GLuint fbo1_ = 0;

    uint frame_count_ = 0;
    uint frame_fps_count_ = 0;
    tick_t frame_fps_tick_ = 0.0;
    float_t frame_fps_ = 0.0;

};

void begin(Env& );
void end(Env& );


} // namespace env


#endif


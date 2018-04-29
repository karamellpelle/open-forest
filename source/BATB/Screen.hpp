//    open-forest: an orienteering game.
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
#ifndef BATB_SCREEN_HPP
#define BATB_SCREEN_HPP
#include "BATB/ModuleBATB.hpp"
#include <GL/glew.h>    // must be done before GLFW!!
#include <GLFW/glfw3.h> // FIXME: build option?


namespace batb
{
namespace screen
{


////////////////////////////////////////////////////////////////////////////////
// 
class Screen : public ModuleBATB
{
public:
    Screen(BATB* b): ModuleBATB( b ) { }

    ////////////////////////////////////////////////////////////////
    // setup
    void begin(const std::string& );
    void end();

    ////////////////////////////////////////////////////////////////
    // frame
    void frameBegin();
    void frameEnd();

    uint frameCount()                               { return frame_count_; }
    float_t frameFPS()                              { return frame_fps_; }

    ////////////////////////////////////////////////////////////////
    // screen
    void getSize(uint& wth, uint& hth) const;
    void getShape(float_t& wth, float_t& hth) const;
    void setFBO0()                                  { setFBO( fbo0_ ); }
    void setFBO1()                                  { setFBO( fbo1_ ); }
    GLuint getFBO() const                           { return fbo_; }
    GLuint setFBO(GLuint );
    void fullscreen(bool ); 
    bool closing();
    void closingClear();

    ////////////////////////////////////////////////////////////////
    // ...

    GLFWmonitor* glfw_monitor   = nullptr;
    GLFWwindow* glfw_window     = nullptr;

private:
    // width before fullscreen
    int nonfullscreen_wth_ = 0;
    int nonfullscreen_hth_ = 0;
    int  nonfullscreen_xpos_ = 0; 
    int  nonfullscreen_ypos_ = 0; 

    ////////////////////////////////////////////////////////////////
    // framebuffer objects
    GLuint fbo_            = 0;
    GLuint fbo0_           = 0;
    GLuint fbo1_           = 0;

    ////////////////////////////////////////////////////////////////
    // statistics
    uint frame_count_      = 0;
    uint frame_fps_count_  = 0;
    tick_t frame_fps_tick_ = 0.0;
    float_t frame_fps_     = 0.0;

};


} // namespace screen

} // namespace batb


#endif


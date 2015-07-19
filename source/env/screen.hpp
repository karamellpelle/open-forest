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
#ifndef ENV_SCREEN_HPP
#define ENV_SCREEN_HPP
#include "env/env_include.hpp"

namespace env
{


// private:
extern YAML::Node doc_;

void screen_begin_();

void screen_end_();

////////////////////////////////////////////////////////////////////////////////
// public:

// the GLFW window
inline GLFWwindow* screen_window()
{
    extern GLFWwindow* screen_window_; 
    return screen_window_;
}


// current FBO for window
inline GLuint screen_fbo()
{
    extern GLuint fbo_current_;
    return fbo_current_;
}

// set custom FBO for window
inline GLuint screen_fbo(GLuint fbo)
{
    extern GLuint fbo_current_;
    
    GLuint prev = fbo_current_;
    // FIXME: glXXX
    fbo_current_ = fbo;
    return prev;
}

// set FBO0 as frambuffer for window, the main framebuffer
inline void screen_fbo0()
{
    extern GLuint fbo_0_;
    screen_fbo( fbo_0_ );
}

// set FBO1 as framebuffer for window, a auxilliary framebuffer (offscreen)
inline void screen_fbo1()
{
    extern GLuint fbo_1_;
    screen_fbo( fbo_1_ );
}

// screen_fboN()


// get size of window, in pixels
inline void screen_size(uint& wth, uint& hth)
{
    int w; int h;
    glfwGetFramebufferSize( screen_window(), &w, &h );

    wth = w;
    hth = h;
}

// get normalized size of window
inline void screen_shape(float_t& wth, float_t& hth)
{
    uint wth_n; uint hth_n;
    screen_size( wth_n, hth_n );

    float_t wth_f = wth_n;
    float_t hth_f = hth_n;
    float_t scale = 1.0 / std::max( wth_n, hth_n );

    wth = wth_f * scale;
    hth = hth_f * scale;
}


}

#endif

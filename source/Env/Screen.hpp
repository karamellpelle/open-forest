//    orienteering-game: an orientering game.
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
#include "Env/Env_include.hpp"



namespace Env
{


void screenBegin();

void screenEnd();

// return 
inline GLuint screenFBO()
{
    extern GLuint fbo_current_;
    return fbo_current_;
}

// set custom FBO for 'Env::Screen'
inline GLuint screenFBOSet(GLuint fbo)
{
    extern GLuint fbo_current_;
    
    GLuint prev = fbo_current_;
    // FIXME: glXXX
    fbo_current_ = fbo;
    return prev;
}

// set FBO0, the main framebuffer (screen)
inline void screenFBOSet0()
{
    extern GLuint fbo_0_;
    screenFBOSet( fbo_0_ );
}

// set FBO1, a auxilliary framebuffer (offscreen)
inline void screenFBOSet1()
{
    extern GLuint fbo_1_;
    screenFBOSet( fbo_1_ );
}


// screenFBOSetN

inline void screenSize(uint& wth, uint& hth)
{
    // FIXME
    wth = 0;
    hth = 0;
}

inline void screenShape(float_t& wth, float_t& hth)
{
    uint wth_n; uint hth_n;
    screenSize( wth_n, hth_n );

    float_t wth_f = wth_n;
    float_t hth_f = hth_f;
    float_t scale = 1.0 / std::max( wth_n, hth_n );

    wth = wth_f * scale;
    hth = hth_f * scale;
}


}

#endif

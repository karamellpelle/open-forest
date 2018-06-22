//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Genergl Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Genergl Public License for more details.
//
//    You should have received a copy of the GNU Genergl Public License glong
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#ifndef BATB_GL_HPP
#define BATB_GL_HPP
#include "BATB/ModuleBATB.hpp"

//#define NANOVG_GL2_IMPLEMENTATION
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg.h"

namespace batb
{

class BATB;
class Scene;


namespace gl
{




class GL : public ModuleBATB
{
public:
    GL(BATB* b) : ModuleBATB( b ) { }

    // setup
    void begin(const std::string& );
    void end();

    // set initial state
    void initState();


    ////////////////////////////////////////////////////////////////
    // working with NanoVG
    // context in pixel coordinates
    // NOTE: not fragment coordinates which is the prime measurement of Scene
    NVGcontext* nanovgBegin(const Scene& );
    void nanovgEnd();  

    // nanovg helpers
    void nanovgNormalize(const Scene& );
    int nanovgFont(const std::string& , const std::string& );
    NVGcontext* nvg_context = nullptr;

    //  GL state for Ogre
    void ogreBegin();
    void ogreEnd();

    // GL state for turbobadger
    void turbobadgerBegin();
    void turbobadgerEnd();


private:
    //std::map<std::string, int> nanovg_fonts_;
    
};


} // namespace gl

} // namespace batb

#endif


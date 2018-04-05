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
#ifndef BATB_GL_GL_HPP
#define BATB_GL_GL_HPP
#include "batb/batb_include.hpp"
#include "batb/ModuleBATB.hpp"

// TODO: GL 3+
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg.h"

namespace batb
{

class Scene;


namespace gl
{




class GL : public ModuleBATB
{
friend void begin(GL& );
friend void end(GL& );

public:
    GL(BATB& b) : ModuleBATB( b ) { }

    NVGcontext* nvg_context = nullptr;

    NVGcontext* nanovg_begin(const Scene& );
    void nanovg_end();  
    void nanovg_normalize(const Scene& );
    int nanovg_font(const std::string& , const std::string& );

private:
    //std::map<std::string, int> nanovg_fonts_;
    
};


void begin(GL& );
void end(GL& );

} // namespace gl

} // namespace batb

#endif


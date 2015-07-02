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
#ifndef BATB_THREAD_GL_CONTEXT_HPP
#define BATB_THREAD_GL_CONTEXT_HPP
#include "batb/batb_include.hpp"

namespace batb
{

class TreadGLContext
{
public:
    class Progress;

    ThreadGLContext();

    void start();
    uint remaining(Progress& current);

    std::vector<Progress> completed() const;


protected:
    void setProgress(float_t , const std::string& = std::string());

private:
    std::vector<Progress> progressions_;

public:
};

class ThreadGLContext::Progress
{
public:
    Progress() { }
    Progress(float_t a, const std::string& n = std::string()) : alpha( a ), name( n ) { }
    
    float_t alpha;
    std::string name;
};



} // namespace batb


#endif


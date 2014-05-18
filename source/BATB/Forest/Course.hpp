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
#ifndef BATB_COURSE_HPP
#define BATB_COURSE_HPP
#include "BATB/Config.hpp"


namespace BATB
{

class Course
{
public:
    Terrain* terrain_;

    // controls in Terrain which this course defines. index 0 is start.
    //[Control*] : 

    // Course does not imply a Map. Terrain can be viewed in different
    // ways, that is, with different Map's. however, given a Course and 
    // a Map for the same Terrain, the Course can be injected into the Map.

    // physical props, i.e. position in Terrain
    // ...

    // length of this course, relative to Terrain measures
    float_t length() const
    {
        return 0.0;
    }
private:
    std::string name_;
    // other meta stuff:
    // ...

    // output stuff
    // GLAnimation anim_;
};


}


#endif

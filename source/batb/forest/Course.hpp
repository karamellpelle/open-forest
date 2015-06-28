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
#ifndef BATB_FOREST_COURSE_HPP
#define BATB_FOREST_COURSE_HPP
#include "batb/forest/Control.hpp"
#include "batb/batb_include.hpp"


namespace batb
{

class BATB;

namespace forest
{

// ways, that is, with different Map's. however, given a Course and 
// a Map for the same Terrain, the Course can be injected into the Map.
class Course
{
public:
    typedef std::forward_list<Control*> ControlList;
   
    // a Course always belongs to a Terrain
    Course(Terrain* t) : terrain_( t )
    {

    }

    Terrain* terrain() const
    {
        return terrain_;
    }

    // load course from XML-definition
    // the XML should define a sequence of points in Terrain,
    // making up the whole course. for each such point, look
    // through the current Control's in Terrain for one in the
    // nearby. if such Control found, add pointer to 'controls_',
    // otherwise create a Control at this point in Terrain.
    //
    // also, we should be able to load a course just from 
    // a sequence of ControlDefinition::Code, that is, 
    // a sequence of codes for Control's in Terrain
    //void load(xml::XMLElement* );



    // length of this course, relative to Terrain measures
    //float_t length() const
    //{
    //    return 0.0;
    //}



private:
   
    ControlList controls_;


    
    // a Course implies a Terrain
    Terrain* const terrain_;

    // meta stuff:
    std::string name_;
    // ...

    // output stuff
    // GLAnimation anim_;
};



} // namespace forest

} // namespace batb


#endif

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
#include "BATB/Forest/ForestKeys.hpp"


namespace BATB
{



void ForestKeys::create(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );
    // FIXME: parse controls from xml...

    // instead, hardcode!
    pointer = createKeyPointer( createKeyMouseAxisX(), createKeyMouseAxisY(),
                                createKeyMouseButton( KeyMouseButton::left ), createKeyMouseButton( KeyMouseButton::right) );

    forward = createKeyButton( GLFW_KEY_W );
    backward = createKeyButton( GLFW_KEY_S );
    left = createKeyButton( GLFW_KEY_A );
    right = createKeyButton( GLFW_KEY_D );
    jump = createKeyButton( GLFW_KEY_SPACE );

}

void ForestKeys::destroy()
{
    pointer = 0;
    forward = 0;
    backward = 0;
    left = 0;
    right = 0;
    jump = 0;

    
    // delet all
    clear();

}

}

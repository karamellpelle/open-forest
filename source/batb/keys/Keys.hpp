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
#ifndef BATB_KEYS_KEYS_HPP
#define BATB_KEYS_KEYS_HPP
#include "batb/batb_include.hpp"

namespace batb
{

class BATB;


namespace keys
{

class Keys
{
friend void begin(Keys& keys);
friend void end(Keys& keys);

public:
    Keys(BATB& b) : batb( b )
    {
    }


    BATB& batb;

    // let this object call back 
    void charCalling(GLFWcharfun );
    void keyCalling(GLFWkeyfun );
    void mousebuttonCalling(GLFWmousebuttonfun );
    void cursorposCalling(GLFWcursorposfun );
    void scrollCalling(GLFWscrollfun );

private:
    bool initialized_ = false;

};


////////////////////////////////////////////////////////////////////////////////
//  

void begin(Keys& keys);

void end(Keys& keys);


} // namespace keys

} // namespace batb


#endif

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
#include "BATB/Keys/Key.hpp"

namespace BATB
{


class KeyButton : public Key
{
public:
    typedef int Code;

    KeyButton(Code c) : code_( c )
    {
    }
    void clear();
    void update(tick_t );
    float_t alpha();

    bool click();
    bool click_double();
    bool pressed();
    bool released();
    bool hold();
    
    // set alpha function
    set_alpha_hold();
    set_alpha_();
private:
    
    Code code_;


};


}

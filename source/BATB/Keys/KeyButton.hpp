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

    KeyButton(Code c);
    void clear();
    void update(tick_t );
    float_t alpha();

    bool click()          { return released_ && 1 <= clicks_; }
    bool click_double()   { return released_ && 2 <= clicks_; }
    uint click_count()    { if ( released_ ) return clicks_; return 0; }

    bool pressed()  { return pressed_; }
    bool released() { return released_; }
    bool press()    { return down_; }
    bool press(tick_t& ticks) { ticks = tick_ - tick_prev_; return down_; }

    float_t press_alpha();
    
    // set alpha function
    //bool press_alpha() { return alpha_; }

private:
    static const uint ticks_clicks_;    
    const Code code_;

    bool down_prev_;
    bool down_;
    tick_t tick_down_;
    tick_t tick_;

    bool pressed_;
    bool released_;
    uint click_count_;

    float_t alpha_;
};


}

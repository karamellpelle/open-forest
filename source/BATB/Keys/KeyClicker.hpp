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
#ifndef BATB_KEYS_KEY_CLICKER_HPP
#define BATB_KEYS_KEY_CLICKER_HPP
#include "BATB/Keys/Key.hpp"

namespace BATB
{


// create click info, for child Key
// FIXME: this is more a container then a Key
class KeyClicker : public Key
{
friend class Keys;
public:
    KeyClicker(Key* c);

    void clear();
    void update(tick_t );
    float_t alpha()           { return key_->alpha(); }

    bool click()              { return released_ && 1 <= click_count_; }
    bool click_double()       { return released_ && 2 == click_count_; }
    uint click_count()        { if ( released_ ) return click_count_; return 0; }

    bool pressed()            { return pressed_; }
    bool pressed(tick_t& ticks)
    {
        if ( pressed() )
        {
            ticks = tick_ - tick_up_;
        }
        return pressed();
    }
    bool released()           { return released_; }
    bool released(tick_t& ticks)
    {
        if ( released() )
        {
            ticks = tick_ - tick_down_;
        }
        return released();
    }
    bool press()              { return down_; }
    bool press(tick_t& ticks)
    {
        if ( press() )
        {
            ticks = tick_ - tick_down_;
        }
        return press();
    }


private:
    static constexpr tick_t ticks_clicks_a_ = 0.2;
    static constexpr tick_t ticks_clicks_b_ = 0.2;    

    Key* const key_;

    bool is_down_()            { return key_->is_down(); } 

    bool down_prev_;
    bool down_;
    tick_t tick_down_;
    tick_t tick_up_;
    tick_t tick_;

    bool pressed_;
    bool released_;
    uint click_count_;
};


}

#endif

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
#include "batb/keys/KeyClicker.hpp"


namespace batb
{

namespace keys
{ 

KeyClicker::KeyClicker(Key* k) : key_( k )
{
    clear();
}


void KeyClicker::clear()
{
    // child
    key_->clear();

    down_prev_ = false;
    down_ = false;
    tick_down_ = 0.0;
    tick_up_ = 0.0;
    tick_ = 0.0;

    pressed_ = false;
    released_ = false;
    click_count_ = 0;

}


void KeyClicker::update(tick_t tick)
{
    // child
    key_->update( tick );

    pressed_ = false;
    released_ = false;

    down_ = is_down_(); 
    tick_ = tick;

    // high
    if ( down_ )
    {
        // pressed
        if ( !down_prev_ )
        {
            pressed_ = true;
            tick_down_ = tick_;
        }

    }
    // low
    else
    {

        // released
        if ( down_prev_ )
        {
            released_ = true;
            tick_up_ = tick_;

            // click: fast press/release
            if ( tick_ <= tick_down_ + ticks_clicks_a_ )
            {
                ++click_count_;
            }
        }

        // reset count of multiple clicks, if fail
        if ( tick_down_ + ticks_clicks_b_ <= tick_ )
        {
            click_count_ = 0;
        }

    }
    down_prev_ = down_;

    
}


} // namespace keys


} // namespace batb

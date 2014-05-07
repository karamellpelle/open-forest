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
#ifndef BATB_KEYS_POINTER_HPP
#define BATB_KEYS_POINTER_HPP
#include "BATB/Keys/Key.hpp"
#include "BATB/Keys/KeyClicker.hpp"


namespace BATB
{


// create a Pointer-key (Mouse-similar)
// FIXME: this is more a container then a Key
class KeyPointer : public Key
{
friend class Keys;
private:
    KeyPointer(Key* x, Key* y, Key* l, Key* r) : axis_x_( x ), axis_y_( y ), clicker_left_( l ), clicker_right_( r )
    {
    }

public:
    void clear()
    {
        axis_x_->clear();
        axis_y_->clear();
        clicker_left_.clear();
        clicker_right_.clear();

    }
    void update(tick_t t)
    {
        axis_x_->update( t );
        axis_y_->update( t );
        clicker_left_.update( t );
        clicker_right_.update( t );
    }
    float_t alpha()
    {
        // FIXME: implement something
        return 0.5;
    }



    void axis(float_t& x, float_t& y)
    {
        x = axis_x_->alpha();
        y = axis_y_->alpha();
    }
    void axis(float wth, float hth, float& x, float& y)
    {
        x = axis_x_->alpha(0, wth);
        y = axis_y_->alpha(0, hth);
    }
    void axis_center(float wth, float hth, float& x, float& y)
    {
        x = axis_x_->alpha( -0.5 * wth, 0.5 * wth );
        y = axis_y_->alpha( -0.5 * hth, 0.5 * hth );
    }
    // FIXME: drag, drop, movement
  
    KeyClicker* left()      { return &clicker_left_; }
    KeyClicker* right()     { return &clicker_right_; }

private:
    Key* const axis_x_;
    Key* const axis_y_;
    KeyClicker clicker_left_;
    KeyClicker clicker_right_;
    
};



}

#endif

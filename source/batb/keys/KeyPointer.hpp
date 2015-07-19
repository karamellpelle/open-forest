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
#ifndef BATB_KEYS_POINTER_HPP
#define BATB_KEYS_POINTER_HPP
#include "batb/keys/Key.hpp"
#include "batb/keys/Keys.hpp"
#include "batb/keys/KeyClicker.hpp"


namespace batb
{

namespace keys
{

// create a Pointer-key (i.e. mouse)
// (this is more a container then a Key)
class KeyPointer : public Key
{

public:
    KeyPointer(Keys& keys, Key* x, Key* y, Key* l, Key* r) : Key( typeid( this ), keys )
    {
        axis_x_ = x;
        axis_y_ = y;
        clicker_left_ = keys_.createKeyClicker( l );
        clicker_right_ = keys_.createKeyClicker( r );
    }

    void reset() override
    {
        axis_x_->reset();
        axis_y_->reset();
        clicker_left_->reset();
        clicker_right_->reset();

    }
    void step(tick_t ) override;
    float_t alpha() override
    {
        // TODO: implement something based on a mouse
        return 0.5;
    }


    
    void axis(float_t& x, float_t& y)
    {
        x = axis_x_->alpha();
        y = axis_y_->alpha();
    }
    void axis(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        x = axis_x_->alpha(0, wth);
        y = axis_y_->alpha(0, hth);
    }
    void axis_center(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        x = axis_x_->alpha( -0.5 * wth, 0.5 * wth );
        y = axis_y_->alpha( -0.5 * hth, 0.5 * hth );
    }


    bool drag(float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
    {
        return drag( 1.0, 1.0, x0, y0, x1, y1, ticks );
    }
    bool drag(float_t wth, float_t hth, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
    {
        return drag( 0.0, wth, 0.0, hth, x0, y0, x1, y1, ticks );
    }
    bool drag_center(float_t wth, float_t hth, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
    {
        return drag( -0.5 * wth, 0.5 * wth, -0.5 * hth, 0.5 * hth, x0, y0, x1, y1, ticks );
    }
    bool drop(float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
    {
        return drop( 1.0, 1.0, x0, y0, x1, y1, ticks );
    }
    bool drop(float_t wth, float_t hth, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
    {
        return drop( 0.0, wth, 0.0, hth, x0, y0, x1, y1, ticks );
    }
    bool drop_center(float_t wth, float_t hth, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
    {
        return drop( -0.5 * wth, 0.5 * wth, -0.5 * hth, 0.5 * hth, x0, y0, x1, y1, ticks );
    }
    
    bool left_pressed(float_t& x, float_t& y)
    {
        return left_pressed( 0.0, 1.0, 0.0, 1.0, x, y );
    }
    bool left_pressed(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return left_pressed( 0.0, wth, 0.0, hth, x, y );
    }
    bool left_pressed_center(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return left_pressed( -0.5 * wth, 0.5 * wth, -0.5 * hth, 0.5 * hth, x, y );
    }
    bool left_released(float_t& x, float_t& y)
    {
        return left_released( 0.0, 1.0, 0.0, 1.0, x, y );
    }
    bool left_released(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return left_released( 0.0, wth, 0.0, hth, x, y );
    }
    bool left_released_center(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return left_released( -0.5 * wth, 0.5 * wth, -0.5 * hth, 0.5 * hth, x, y );
    }
    bool right_pressed(float_t& x, float_t& y)
    {
        return right_pressed( 0.0, 1.0, 0.0, 1.0, x, y );
    }
    bool right_pressed(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return right_pressed( 0.0, wth, 0.0, hth, x, y );
    }
    bool right_pressed_center(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return right_pressed( -0.5 * wth, 0.5 * wth, -0.5 * hth, 0.5 * hth, x, y );
    }
    bool right_released(float_t& x, float_t& y)
    {
        return right_released( 0.0, 1.0, 0.0, 1.0, x, y );
    }
    bool right_released(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return right_released( 0.0, wth, 0.0, hth, x, y );
    }
    bool right_released_center(float_t wth, float_t hth, float_t& x, float_t& y)
    {
        return right_released( -0.5 * wth, 0.5 * wth, -0.5 * hth, 0.5 * hth, x, y );
    }
  
    KeyClicker* left()      { return clicker_left_; }
    KeyClicker* right()     { return clicker_right_; }

private:
    bool drag(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks);
    bool drop(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks);
    bool left_pressed(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x, float_t& y)
    {
        if ( left()->pressed() )
        {
            x = axis_x_->alpha(x_a, x_b);
            y = axis_y_->alpha(y_a, y_b);
        }
        return left()->pressed();
    }
    bool left_released(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x, float_t& y)
    {
        if ( left()->released() )
        {
            x = axis_x_->alpha(x_a, x_b);
            y = axis_y_->alpha(y_a, y_b);
        }
        return left()->released();

    }
    bool right_pressed(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x, float_t& y)
    {
        if ( right()->pressed() )
        {
            x = axis_x_->alpha(x_a, x_b);
            y = axis_y_->alpha(y_a, y_b);
        }
        return right()->pressed();
    }
    bool right_released(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x, float_t& y)
    {
        if ( right()->released() )
        {
            x = axis_x_->alpha(x_a, x_b);
            y = axis_y_->alpha(y_a, y_b);
        }
        return right()->released();

    }

    // TODO: public
    Key* axis_x_;
    Key* axis_y_;
    KeyClicker* clicker_left_;
    KeyClicker* clicker_right_;

    float_t x0_;
    float_t y0_;
    
};


} // namespace keys

} // namespace batb

#endif

//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
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
#include "BATB/Keys/KeyPointer.hpp"
#include "BATB/Keys/KeyMouseButton.hpp"
#include "BATB/Keys/KeyMouseAxis.hpp"
#include "BATB/Keys/KeyClicker.hpp"


namespace batb
{

namespace keys
{

KeyPointer::KeyPointer(BATB* b, Key* x, Key* y, Key* l, Key* r) : Key( b )
{
    // create default if not provided
    axis_x        = x ? x : (Key*)( batb->keys->create<KeyMouseAxis>( code::MouseAxis::X ) );
    axis_y        = y ? y : (Key*)( batb->keys->create<KeyMouseAxis>( code::MouseAxis::Y ) );
    left  = batb->keys->create<KeyClicker>( l ? l : (Key*)( batb->keys->create<KeyMouseButton>( GLFW_MOUSE_BUTTON_LEFT ) ) );
    right = batb->keys->create<KeyClicker>( r ? r : (Key*)( batb->keys->create<KeyMouseButton>( GLFW_MOUSE_BUTTON_LEFT ) ) );

}

KeyPointer::KeyPointer(BATB* b) : KeyPointer( b, nullptr, nullptr, nullptr, nullptr )
{

}

void KeyPointer::step(tick_t t)
{
    // dont do this; all Key's are stepped by the Keys object:
    //axis_x_->step( t );
    //axis_y_->step( t );
    //left->step( t );
    //right->step( t );

    if ( left->pressed() )
    {
        x0_ = axis_x->alpha();
        y0_ = axis_y->alpha();
    }


}

float_t KeyPointer::alpha()
{
    // TODO: implement something based on a mouse
    // a KeyPointer is more a container
    return 0.5;
}

void KeyPointer::reset()
{
    axis_x->reset();
    axis_y->reset();
    left->reset();
    right->reset();
}

void KeyPointer::canDisable(bool b) 
{
    // all called; is this correct behaviour?
    axis_x->canDisable( b );
    axis_y->canDisable( b );
    left->canDisable( b );
    right->canDisable( b );
}
////////////////////////////////////////////////////////////////////////////////
//

bool KeyPointer::drag(float_t x_a, float_t x_b, float_t y_a, float_t y_b, float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
{
    if ( left->press( ticks ) )
    {
        x0 = (1.0 - x0_) * x_a + x0_ * x_b;
        y0 = (1.0 - y0_) * y_a + y0_ * y_b;

        float_t x1_ = axis_x->alpha();
        float_t y1_ = axis_y->alpha();
        x1 = (1.1 - x1_) * x_a + x1_ * x_b;
        y1 = (1.1 - y1_) * y_a + y1_ * y_b;
         
        return true;
    }
    return false;
}


bool KeyPointer::drop(float_t x_a, float_t x_b, float_t y_a, float_t y_b,  float_t& x0, float_t& y0, float_t& x1, float_t& y1, tick_t& ticks)
{
    if ( left->released( ticks ) )
    {
        x0 = (1.0 - x0_) * x_a + x0_ * x_b;
        y0 = (1.0 - y0_) * y_a + y0_ * y_b;

        float_t x1_ = axis_x->alpha();
        float_t y1_ = axis_y->alpha();
        x1 = (1.1 - x1_) * x_a + x1_ * x_b;
        y1 = (1.1 - y1_) * y_a + y1_ * y_b;
         

        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// unique identifier
std::string KeyPointer::name() const
{
    std::ostringstream os; os << "KeyPointer";
    return os.str();
}
// human readable
std::string KeyPointer::nameEasy() const
{
    return "mouse";
}

// GUI widget
std::string KeyPointer::nameGUI() const
{
    // TODO: image
    std::ostringstream os; os << "<widget TBButton: text: 'mouse'>";
    return os.str();
}

} // namespace keys

} // namespace batb

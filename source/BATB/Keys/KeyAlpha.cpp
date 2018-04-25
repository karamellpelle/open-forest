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
#include "BATB/Keys/KeyAlpha.hpp"

namespace batb
{

namespace keys
{

void KeyAlpha::step(tick_t tick) 
{
    // FIXME: prevent too big dt, espcecially first time...
    tick_t dt = tick - tick_prev_;           // assuming dt not too smal, i.e. when high frame rate and dt ~= 0...
    float_t x = key_->alpha();
    alpha_ +=  (tick_t)( dt * (tick_t)( a_ * (1.0 - x) + b_ * x ) );
    alpha_ = clamp( alpha_, (float_t)0.0, (float_t)1.0 );
    tick_prev_ = tick;
}

void KeyAlpha::set(float_t a, float_t b)
{
    a_ = a;
    b_ = b;
}

void KeyAlpha::set_clear(float_t clear)
{
    clear_ = clear;
}
////////////////////////////////////////////////////////////////////////////////
// names. default.

// unique identifier
std::string KeyAlpha::name() const
{
    // TODO: think through
    std::ostringstream os; os << "KeyAlpha";
    return os.str();
}
// human readable
std::string KeyAlpha::nameEasy() const
{
    return "";
}

// GUI widget
std::string KeyAlpha::nameGUI() const
{
    std::ostringstream os; os << "<widget TBButton: text: '" << nameEasy() << "'>";
    return os.str();
}

} // namespace keys

} // namespace batb

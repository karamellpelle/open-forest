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
#ifndef BATB_KEYS_KEYSCROLL_HPP
#define BATB_KEYS_KEYSCROLL_HPP
#include "BATB/Keys/Key.hpp"
#include "BATB/Value/Keys.hpp"
#include "helpers/follow_value.hpp"


namespace batb
{

namespace keys
{


class KeyMouseScroll : public Key
{

public:
    KeyMouseScroll(Keys* );

    void    reset() override;
    void    step(tick_t ) override;
    float_t alpha() override;

    virtual std::string name()     const override; // unique identifier
    virtual std::string nameEasy() const override; // human readable
    virtual std::string nameGUI()  const override; // GUI widget

    // update
    void    add(double n);

private:
    //double scroll_n_      = 0.0;
    //double scroll_unit_   = 0.0;

    follow_value<double> follow_;
};

////////////////////////////////////////////////////////////////////////////////
//

inline KeyMouseScroll::KeyMouseScroll(Keys* keys) : Key( keys )
{
    auto unit = value::keyMouseScrollUnit;
    auto vel  = value::keyMouseScrollVel;

    follow_.speed( vel / unit );

}

inline void KeyMouseScroll::reset()
{
    follow_.clear();
}

inline void KeyMouseScroll::step(tick_t t)
{
    follow_.step( t );
}

inline float_t KeyMouseScroll::alpha()
{
    //std::cout << "scroll! current: " << follow_.get() << ", ideal: " << follow_.ideal << "\r" << std::flush;
    return follow_.get();
}

inline void KeyMouseScroll::add(double n)
{
    follow_.ideal += n / value::keyMouseScrollUnit;
}

} // namespace keys

} // namespace batb
#endif

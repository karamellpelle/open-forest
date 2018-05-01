//    open-forest: an orienteering game.
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
#ifndef HELPERS_TB_HPP
#define HELPERS_TB_HPP
#include "include.hpp"
#include "tb/tb_widgets.hpp"


// FIXME: better output. 1 line

std::ostream& operator<<(std::ostream& os, const TBRect& r)
{
    os << "x: " << r.x << "\n";
    os << "y: " << r.y << "\n";
    os << "w: " << r.w << "\n";
    os << "h: " << r.h << "\n";
    return os;
}
std::ostream& operator<<(std::ostream& os, const LayoutParams& lp)
{
    os << "pref_w: " << lp.pref_w << "\n";
    os << "min_w:  " << lp.min_w << "\n";
    os << "max_w:  " << lp.max_w << "\n";
    os << "pref_h: " << lp.pref_h << "\n";
    os << "min_h:  " << lp.min_h << "\n";
    os << "max_h:  " << lp.max_h << "\n";
    return os;
}
std::ostream& operator<<(std::ostream& os, const PreferredSize& pref)
{
    os << "pref_w: " << pref.pref_w << "\n";
    os << "min_w:  " << pref.min_w << "\n";
    os << "max_w:  " << pref.max_w << "\n";
    os << "pref_h: " << pref.pref_h << "\n";
    os << "min_h:  " << pref.min_h << "\n";
    os << "max_h:  " << pref.max_h << "\n";
    return os;
}


namespace tb
{

}


#endif


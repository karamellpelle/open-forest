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
#include "BATB/Keys/KeyMouseAxis.hpp"

namespace batb
{

namespace keys
{
////////////////////////////////////////////////////////////////////////////////
// names. default.

// unique identifier
std::string KeyMouseAxisX::name() const
{
    // TODO: think through
    std::ostringstream os; os << "KeyMouseAxisX";
    return os.str();
}
// human readable
std::string KeyMouseAxisX::nameEasy() const
{
    return "mouse";
}

// GUI widget
std::string KeyMouseAxisX::nameGUI() const
{
    std::ostringstream os; os << "<widget TBButton: text: 'mouse'>";
    return os.str();
}

// unique identifier
std::string KeyMouseAxisY::name() const
{
    std::ostringstream os; os << "KeyMouseAxisY";
    return os.str();
}
// human readable
std::string KeyMouseAxisY::nameEasy() const
{
    return "mouse";
}

// GUI widget
std::string KeyMouseAxisY::nameGUI() const
{
    // TODO: image
    std::ostringstream os; os << "<widget TBButton: text: 'mouse'>";
    return os.str();
}

} // namespace keys

} // namespace batb

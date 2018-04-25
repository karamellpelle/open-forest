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
#ifndef BATB_KEYS_CODE_HPP
#define BATB_KEYS_CODE_HPP
#include "include.hpp"
#include <GLFW/glfw3.h>

namespace batb
{
namespace keys
{

namespace code
{

// these are GLFW codes
using KeyboardButton = int;
using MouseButton = int;
using ButtonState = int;


// these are custom
enum class MouseScroll
{
    X,
    Y
};
enum class MouseAxis
{
    X,
    Y
};



} // namespace code


} // namespace keys

} // namespace batb


#endif

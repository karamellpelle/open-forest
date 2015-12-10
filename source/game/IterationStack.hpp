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
#ifndef GAME_ITERATION_STACK_HPP
#define GAME_ITERATION_STACK_HPP
#include <forward_list>


namespace game
{

template <typename A>
class Iteration;


// define stack type (forward list)
// TODO: forward_list<shared_ptr<A>> ?
template <typename A>
using IterationStack = std::forward_list<Iteration<A>*>;

// there is a bug for empty braces initialization in c++11 :(
//
// this should be fixed in c++14, but not for my for gcc libstdc++ yet:
// http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#2193
//
// unreleased source of gcc shows the correct implementation, so emtpy
// braces initialization will work in the future for c++14 on gcc too
// 
// also see: http://stackoverflow.com/questions/24510067/why-does-return-not-apply-to-stdforward-list
//
// instead, for now,
// a dirty fix, when inside implementation of 'iterate':
#define _ game::IterationStack<World>()



} // namespace game

#endif


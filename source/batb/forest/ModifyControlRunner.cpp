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
#include "batb/forest/ModifyControlRunner.hpp"
#include "batb/forest/ModifyRunner.hpp"
#include "batb/forest.hpp"
#include "batb.hpp"


namespace batb
{

namespace forest
{

// controlling runner of ModifyRunner
void ModifyControlRunner::modifier(ModifyRunner* mod)
{
    modifier_ = mod;
}

void ModifyControlRunner::operator()(World& forest)
{
    if ( modifier_ )
    {
        // TODO
        // here:
        // * use aiming as we do in ModifyControlCamera
        // * Forest::Keyset keys left, right (i.e. keyboard W and D) 
        //   modifies aiming a to left and right (i.e. z column)
        //   use: modifier->aim()
        // * Forest::Keyset forward, backward (i.e. keyboard W and S)
        //   sets speed 1 and -1
        //   use: modifier->speed()
        //
    }

}

} // namespace forest

} // namespace batb




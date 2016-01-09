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
#include "batb.hpp"
#include "batb/BATB.hpp"
#include "batb/run/Run.hpp"
#include "batb/run/Console.hpp"
#include "batb/run/Console/parse.hpp"

#include "batb/value/batb.hpp"
#include "batb/value/run.hpp"
#include "batb/value/forest.hpp"

 


namespace batb
{

namespace run
{


// modify value::'s
bool cmd_value(BATB& batb, std::string& in)
{
    // (allowed to modify 'in' directly)
    return true;
}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


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
#include "BATB/Run.hpp"
#include "BATB/Run/Console.hpp"
#include "BATB/Run/Console/parse.hpp"



  
namespace batb
{

namespace run
{

// output string to console
bool cmd_echo(BATB* batb, std::string& in)
{
    // (allowed to modify 'in' directly)
    // TODO: parse commands (-n for no newline, etc.)

    // eat string literal
    std::string str;
    if ( stringliteral( str, in ) ) 
    {
        batb->run->console << str << std::endl;
        return true;
    }

    // else just output whatever on the line
    batb->run->console << in << std::endl;

    return false;
    
}

////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


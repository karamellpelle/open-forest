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
#include "BATB/AL.hpp"



  
namespace batb
{

namespace run
{

// output string to console
bool cmd_easteregg(BATB* batb, const std::string& input)
{
    // ('input' is the whole command line typed)
  
    // if correct command typed, do easter egg
    // easter egg is PS1|reverse(PS1)
    

    if ( input.empty() )         return false;
    if ( input.front() != '|' )  return false;
    auto right = input.substr( 1 );
    std::reverse( std::begin( right ), std::end( right ) );
    for (auto i = std::begin( right ); i != std::end( right ); ++i)
    {
        if ( *i == '<' ) { *i = '>'; continue; }
        if ( *i == '>' ) { *i = '<'; continue; }
        if ( *i == '(' ) { *i = ')'; continue; }
        if ( *i == ')' ) { *i = '('; continue; }
        if ( *i == '[' ) { *i = ']'; continue; }
        if ( *i == ']' ) { *i = '['; continue; }
        if ( *i == '{' ) { *i = '}'; continue; }
        if ( *i == '}' ) { *i = '{'; continue; }
    }

    auto left   = batb->run->console->getPS1();
    
    if ( left == right )
    {
        auto src = batb->al->play2D( file::static_data( "easteregg.mp3" ) );

        return true;
    }
    return false;
    
}

////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


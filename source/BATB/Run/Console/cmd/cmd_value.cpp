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
#include "BATB/Value.hpp"
#include "BATB/Run/Console.hpp"
#include "BATB/Run/Console/parse.hpp"

#include "BATB/Value/BATB.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Value/Forest.hpp"

 


namespace batb
{

namespace run
{


// modify value::'s
bool cmd_value(BATB* batb, std::string& in)
{
    auto name = identifier( in );

    auto v = word( in );

    // get value if nothing follows the variable name
    if ( v.empty() )
    {
        // get
        std::string value;
        if ( batb->value->get( name, value ) )
        {
            batb->run->console << value << "\n";
            return true;
        }
        else
        {
            batb->run->console << "variable '"
                               << name 
                               << "' is not available.\n";
            return false;
        }
    }
    else
    {
        if ( batb->value->set( name, v ) )
        {
            return true;
        }
        else
        {
            batb->run->console << "could not set variable '"
                               << name
                               << "'\n"
                               ;
            return false;
        }
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


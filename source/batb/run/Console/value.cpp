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
#include "batb/run/Run.hpp"
#include "batb/run/Console.hpp"
#include "batb/run/Console/parse.hpp"

#include "batb/value/batb.hpp"
#include "batb/value/run.hpp"
#include "batb/value/forest.hpp"

//#define TRY_SET(name)
 


namespace batb
{

namespace run
{


bool set_value(const std::string& name, const std::string& value)
{
    // TODO: use BATB::Value!
    
    return false;

}

// modify value::'s
void command_value(Console& console, std::string& in)
{
    // (allowed to modify 'in' directly)
    // TODO 
    if ( word( "set", in ) )
    {
        auto name = word( in );
        auto value = word( in );

        if ( !set_value( name, value ) )
        {
            console.batb.log << "run::Console: could not set value '" << name << "'";
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


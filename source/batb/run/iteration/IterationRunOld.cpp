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
#include "batb/old.hpp"

namespace batb
{

namespace run
{


IterationRunOld::IterationRunOld(BATB& b) : IterationRun( b )
{

}


void IterationRunOld::iterate_begin(World& world)
{
    BATB_LOG_FUNC( batb );

    old::begin();

}


void IterationRunOld::iterate_run(IterationStack& stack, World& world)
{
    if ( old::is_exit() )
    {
        old::end();
        return stack.finish();
    }
    else
    {
        // do
        old::iterate();

        // think
        if ( batb.run.keyset.old->released() )
        {
            batb.log << "IterationRunOld -> " << std::endl;
            old::end();

            return stack.finish();
        }
        else
        {
            return stack.next( this );
        }
    }




}


////////////////////////////////////////////////////////////////////////////////
//

void begin(IterationRunOld& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

void end(IterationRunOld& iter)
{
    
    BATB_LOG_FUNC( iter.batb );
}


} // namespace run

} // namespace batb










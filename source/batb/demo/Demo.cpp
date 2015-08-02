//    open-demo: an orientering game.
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
#include "batb/demo/Demo.hpp" 
#include "batb.hpp" 

namespace batb
{



namespace demo
{


////////////////////////////////////////////////////////////////////////////////
//  Demo

Demo::Demo(BATB& b) : ModuleBATB( b ), keyset( b ),
                          iterationDemoDemo( b )
{

}



////////////////////////////////////////////////////////////////////////////////
// 
void begin(Demo& demo)
{

    BATB_LOG_FUNC( demo.batb );
    
    if ( demo.init_empty() )
    {
        // load associated keys 
        demo.keyset.load( file::dynamic_data( "batb/demo/KeySet.yaml" ) ); // TODO: from yaml!!

        // begin iterations:
        demo::begin( demo.iterationDemoDemo );

    }

    demo.init( true );
}

void end(Demo& demo)
{
    BATB_LOG_FUNC( demo.batb );

    if ( demo.init_nonempty() )
    {
        demo.save();

        // end  iterations:
        demo::end( demo.iterationDemoDemo );

    }
    
    demo.init( false );
}


} // namespace demo

} // namespace batb



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
#include "batb/forest/Forest.hpp" 
#include "batb.hpp" 

namespace batb
{



namespace forest
{


////////////////////////////////////////////////////////////////////////////////
//  Forest

Forest::Forest(BATB* b) : ModuleBATB( b )
{
    keyset = std::make_unique<KeySet>( b );
}



////////////////////////////////////////////////////////////////////////////////
// 
void Forest::begin(const std::string& path)
{

    BATB_LOG_FUNC( batb );
    
    if ( init_empty() )
    {
        config( path );

        // load associated keys 
        keyset->load( file::dynamic_data( "batb/forest/KeySet.yaml" ) );

    }

    init( true );
}

void Forest::end()
{
    BATB_LOG_FUNC( batb );

    if ( init_nonempty() )
    {
        save();

    }
    
    init( false );
}


} // namespace forest

} // namespace batb



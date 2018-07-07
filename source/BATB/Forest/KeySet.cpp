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
#include "BATB/Forest/KeySet.hpp"
#include "helpers/yaml.hpp"


namespace batb
{
namespace forest
{



KeySet::KeySet(BATB* b) : keys::KeySet( b )
{

}



void KeySet::load(const YAML::Node& yaml)
{

    using namespace keys;

    // TODO: release current pointers back to Keys

    batb->log << "forward:  ";
    forward = create<KeyClicker>( createSafeKey( yaml > "forward" ) );
    batb->log->endl();

    batb->log << "backward: ";
    backward = create<KeyClicker>( createSafeKey( yaml > "backward" ) );
    batb->log->endl();

    batb->log << "left:     ";
    left = create<KeyClicker>( createSafeKey( yaml > "left" ) );
    batb->log->endl();

    batb->log << "right:    ";
    right = create<KeyClicker>( createSafeKey( yaml > "right" ) );
    batb->log->endl();

    batb->log << "aim:      ";
    aim = create<KeyPointer>( createKey( yaml > "aim" > "KeyPointer" > "axis-x" ),
                              createKey( yaml > "aim" > "KeyPointer" > "axis-y" ),
                              createKey( yaml > "aim" > "KeyPointer" > "left" ),
                              createKey( yaml > "aim" > "KeyPointer" > "right" )
                            );
    batb->log->endl();

    mapview       = create<KeyClicker>( createSafeKey( yaml > "mapview" ) ); 
    mapview_plus  = create<KeyClicker>( createSafeKey( yaml > "mapview-plus" ) );  
    mapview_shift = create<KeyClicker>( createSafeKey( yaml > "mapview-shift" ) );  
    mapview_ctrl  = create<KeyClicker>( createSafeKey( yaml > "mapview-ctrl" ) );  

    ///mapview_rotate       = nullptr; // FIXME

    ////////////////////////////////////////////////////////////////

    tmp0 = create<KeyClicker>( create<KeyKeyboardButton>( GLFW_KEY_V ) );
    tmp1 = create<KeyClicker>( create<KeyKeyboardButton>( GLFW_KEY_B ) );

}

void KeySet::save(YAML::Node& yaml)
{

}

void KeySet::reset()
{
    forward->reset();
    backward->reset();
    left->reset();
    right->reset();
    aim->reset();

    tmp0->reset();
    tmp1->reset();
}

} // namespace forest

} // namespace batb


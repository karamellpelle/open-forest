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

    // TODO: release current pointers back to Keys

    batb->log << "forward:  ";
    forward = createKeyClicker( safeKey( createKey( yaml["forward"] ) ) );
    batb->log << "" << std::endl;

    batb->log << "backward: ";
    backward = createKeyClicker( safeKey( createKey( yaml["backward"] ) ) );
    batb->log << "" << std::endl;

    batb->log << "left:     ";
    left = createKeyClicker( safeKey( createKey( yaml["left"] ) ) );
    batb->log << "" << std::endl;

    batb->log << "right:    ";
    right = createKeyClicker( safeKey( createKey( yaml["right"] ) ) );
    batb->log << "" << std::endl;

    batb->log << "aim:      ";
    aim = safeKey( createKeyPointer( yaml > "aim" > "KeyPointer" ) );
    batb->log << "" << std::endl;

    ////////////////////////////////////////////////////////////////

    tmp0 = createKeyClicker( createKeyKeyboardButton( GLFW_KEY_V ) );
    tmp1 = createKeyClicker( createKeyKeyboardButton( GLFW_KEY_B ) );

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


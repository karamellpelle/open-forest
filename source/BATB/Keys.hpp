//    orienteering-game: an orientering game.
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

#include "BATB/Config.hpp"


namespace BATB
{

class Keys
{
public:
    static void create(Config::Block* );
    static void destroy();

    // clear all keys:
    void clear();
    // update all keys:
    void update(tick_t );     

    // Key's to be used in game, typicall for ForestWorld (since RunWorld is
    // typically controlled by GUI)
    //
    // TODO: create classes KeyXXXAbstraction, constructing KeyXXXAbstraction
    //       from Key*, with the similar functionality as KeyXXX. Then use
    //       XML file to define the actual key binding. later, only use the 
    //       abstraction Key for control.
    //       
    KeyMouse*  mouse;
    KeyButton* button_pause;
    KeyButton* button_forward;
    KeyButton* button_backward;
    KeyButton* button_left;
    KeyButton* button_right;
    KeyButton* button_jump;

private:
    Keys() { }

    // abstraction
    //Key* key_create(Config::Block* );
    //std::vector<Key*> keys_allocations_;
};

Keys* keys();

}

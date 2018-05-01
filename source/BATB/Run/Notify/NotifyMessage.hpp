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
#ifndef BATB_RUN_NOTIFY_NOTIFYMESSAGE_HPP
#define BATB_RUN_NOTIFY_NOTIFYMESSAGE_HPP
#include "BATB/Keys.hpp"



namespace batb
{
class BATB;

namespace run
{
class World;
class Notify;



// representing a message to the user.
// it is presented to the user by a GUI widget, not here
class NotifyMessage
{
friend class Notify;

public:
    NotifyMessage() = default;
    NotifyMessage(const std::string& s) : str( s ) { }
    NotifyMessage(const std::string& s, tick_t d) : str( s ), duration( d ) { }

    void finish() { finished_ = true; }
    
    // the massage string
    // allows embedded content like widgets
    std::string str;


    tick_t tick     = 0;
    tick_t duration = 0;

    // TODO: types. for example tip, warning, private message, next song playing etc.
    bool alert      = false;
    keys::Key* key  = nullptr;
    // etc.

private:
    bool finished_ = false;

};


} // namespace run

} // namespace batb

#endif

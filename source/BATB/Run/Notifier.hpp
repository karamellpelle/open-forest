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
#ifndef BATB_RUN_NOTIFIER_HPP
#define BATB_RUN_NOTIFIER_HPP
#include "BATB/Run/Notifier/NotifyMessage.hpp"
#include "BATB/Run/Notifier/TBNotify.hpp"


namespace batb
{
class BATB;

namespace run
{


// class for presenting short messages to the user, for example
// tips and tricks, in-game tutorials, etc.
class Notifier 
{
public:
    Notifier(BATB* b) : batb( b ) { }

    void step(World& );

    void begin();
    void end();

    // hide and clear
    void clear();

    // show message
    bool message(const NotifyMessage& msg);

    BATB* batb;



private:
    std::list<NotifyMessage> messages_;
    TBNotify* tb_notify_ = nullptr;


};



////////////////////////////////////////////////////////////////////////////////

namespace event
{

// tell that event was completed
class NotifyMessageComplete
{
public:
    NotifyMessageComplete(NotifyMessage* m) : message( m ) { }

    NotifyMessage* message = nullptr;

};

} // namespace event

} // namespace run

} // namespace batb

#endif

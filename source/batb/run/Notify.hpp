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
#ifndef BATB_RUN_NOTIFY_HPP
#define BATB_RUN_NOTIFY_HPP
#include "batb/batb_include.hpp"


namespace batb
{
class BATB;

namespace run
{
class World;
class Notify;


class NotifyMessage
{
friend class Notify;

public:
    NotifyMessage() = default;
    NotifyMessage(const std::string& s) : str( s ) { }

    std::string str;    // this is also allowed to contain widgets etc.

private:
    tick_t tick_ = 0;
    tick_t duration_ = 0;

};


// class for presenting short messages to the user, for example
// tips and tricks, in-game tutorials, etc.
class Notify 
{
friend void begin(Notify& );
friend void end(Notify& );

public:
    Notify(BATB& b) : batb( b ) { }

    void step(World& );

    // hide and clear
    void clear();

    // show message
    bool operator()(const NotifyMessage& msg);
    template <typename... Args> 
    bool operator()(const Args&... args) { return operator()( NotifyMessage( args... ) ); }


    BATB& batb;


    //TBNotify* tb_notify = nullptr;

private:
    std::forward_list<NotifyMessage> messages_;


};


////////////////////////////////////////////////////////////////////////////////

void begin(Notify& );

void end(Notify& );

////////////////////////////////////////////////////////////////////////////////


} // namespace run

} // namespace batb

#endif

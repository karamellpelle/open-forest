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
#ifndef BATB_RUN_TBNOTIFY_HPP
#define BATB_RUN_TBNOTIFY_HPP
#include "batb/batb_include.hpp"
#include "batb/run/notify/NotifyMessage.hpp"
#include "tb/tb_widgets.h"
#include "tb/tb_layout.h"


namespace tb { class TBEditField; }

namespace batb
{
class BATB;

namespace run
{
class Notify;
class TBNotify;
class World;

// TurboBadger representation (UI) of NotifyMessage
class TBNotifyMessage : public tb::TBWidget
{

public:
    TBOBJECT_SUBCLASS( TBNotifyMessage, tb::TBWidget );
    TBNotifyMessage(TBNotify* , NotifyMessage* );

    TBNotify* tb_notify    = nullptr;
    tb::TBEditField* edit  = nullptr;
    NotifyMessage* message = nullptr;
};



////////////////////////////////////////////////////////////////////////////////

// holding messages
class TBNotify : public tb::TBLayout
{

public:
    TBOBJECT_SUBCLASS( TBNotify, tb::TBLayout );
    TBNotify(BATB& );

    void step(World& );
    void push(NotifyMessage* );

    BATB& batb;

private:
    std::list<TBNotifyMessage*> tb_notify_messages_;
    Notify& notify_;
    tick_t tick_ = 0;
};





////////////////////////////////////////////////////////////////////////////////


} // namespace run

} // namespace batb

#endif

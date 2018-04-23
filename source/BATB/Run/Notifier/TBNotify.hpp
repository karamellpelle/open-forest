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
#ifndef BATB_RUN_TBNOTIFY_HPP
#define BATB_RUN_TBNOTIFY_HPP
#include "BATB/Run/Notifier/NotifyMessage.hpp"
#include "tb/tb_widgets.h"
#include "tb/tb_layout.h"
#include "tb/tb_window.h"


namespace batb
{
class BATB;

namespace run
{
class Notifier;
class NotifyMessage;
class World;
}
}


namespace tb 
{

class TBEditField;
class TBNotify;


////////////////////////////////////////////////////////////////////////////////
// this is a widget showing notifications
//
class TBNotifyMessage : public TBWidget
//class TBNotifyMessage : public tb::TBWindow
{

public:
    TBOBJECT_SUBCLASS( TBNotifyMessage, tb::TBWidget );

    TBNotifyMessage() = default;
    
    void OnInflate(const INFLATE_INFO& ) override;

    // set message
    void message(batb::run::NotifyMessage* );

    batb::run::NotifyMessage* notifymessage = nullptr;
    TBNotify* tb_notify                     = nullptr;
    TBEditField* tb_editfield               = nullptr;
};




////////////////////////////////////////////////////////////////////////////////
// this is the parent of notification widgets,
// it's a layout thats holds them aligned
//
class TBNotify : public TBLayout
{

public:
    TBOBJECT_SUBCLASS( TBNotify, TBLayout );

    TBNotify() = default;     // empty constructor needed by TBWidgetFactory
    TBNotify(batb::BATB* );

    void step(batb::run::World& );
    void push(batb::run::NotifyMessage* );

    batb::BATB* batb;

private:
    std::list<TBNotifyMessage*> tb_notify_messages_; // TODO: look at children of tb
    batb::run::Notifier* notify_ = nullptr;
    tick_t tick_ = 0;
};


} // namespace tb







#endif

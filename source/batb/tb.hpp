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
#ifndef BATB_TB_HPP
#define BATB_TB_HPP
#include "batb/batb_include.hpp"
#include "tb/tb_editfield.h"


namespace tb
{

// ignores enter and returns false (hence parent can receive enter)
class TBEditFieldEnter : public TBEditField
{
    TBOBJECT_SUBCLASS( TBEditFieldEnter, TBEditField );

    virtual bool OnEvent(const tb::TBWidgetEvent& event) override
    {
        if ( event.special_key == TB_KEY_ENTER )
        {
            // TBEditField only uses EVENT_TYPE_KEY_DOWN for input.
            // hence return true to dismiss key up, and false
            // for key down (so parent can handle enter event)
            return event.type == EVENT_TYPE_KEY_DOWN;
        }

        return TBEditField::OnEvent( event );
    }
    
};



} // namespace tb

#endif

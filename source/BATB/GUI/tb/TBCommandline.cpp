//    open-forest: an orienteering game.
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
#include "BATB/GUI/tb/TBCommandline.hpp"
#include "tb/tb_window.h"
#include "tb/tb_widgets_reader.h"


namespace tb
{


void TBCommandline::OnInflate(const tb::INFLATE_INFO& info)
{
    TBEditField::OnInflate( info );
}


bool TBCommandline::OnEvent(const tb::TBWidgetEvent& event)
{
    // prevent special usage of UP/DOWN (StyleEdit movements)
    if ( event.type == tb::EVENT_TYPE_KEY_DOWN )
    {
        if ( event.special_key == tb::TB_KEY_UP || event.special_key == tb::TB_KEY_DOWN )
        {
            // propagate event to parent  
            return false;
        }
    }

    // otherwise act like an EditField
    return TBEditField::OnEvent( event );

}


TB_WIDGET_FACTORY(TBCommandline, TBValue::TYPE_STRING, WIDGET_Z_TOP) {}



} // namespace tb




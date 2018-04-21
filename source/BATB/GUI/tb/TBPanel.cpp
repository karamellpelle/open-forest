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
#include "BATB/GUI/tb/TBPanel.hpp"
#include "tb/tb_window.h"
#include "tb/tb_widgets_reader.h"


namespace tb
{


////////////////////////////////////////////////////////////////////////////////
// TBPanel (a window)

TBPanel::TBPanel()
{
    SetSettings( tb::WINDOW_SETTINGS_RESIZABLE );
    
}


void TBPanel::OnInflate(const tb::INFLATE_INFO& info)
{
    //TBWindow::OnInflate( info );
    TBWidget::OnInflate( info );

    //if (const char *skin = info.node->GetValueString("skin", nullptr))
    //{
    //    SetSkinBg(skin);
    //}

}

// TODO: remove this
bool TBPanel::OnEvent(const tb::TBWidgetEvent& event)
{
    return false;
}

TB_WIDGET_FACTORY(TBPanel, TBValue::TYPE_STRING, WIDGET_Z_TOP) {}


} // namespace tb




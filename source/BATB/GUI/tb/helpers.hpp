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
#ifndef BATB_RUN_GUI_TB_HELPERS_HPP
#define BATB_RUN_GUI_TB_HELPERS_HPP
#include "tb/tb_widgets.h"
#include "tb/tb_widgets_reader.h"

namespace tb
{


////////////////////////////////////////////////////////////////////////////////
// create a widget for us from node and adding it to 'parent'. see
// TBWidget::OnInflate() and TBWidgetsReader::CreateWIdget()
// in tb_widgets_reader.cpp

template <typename Widget>
Widget* new_widget(TBWidget* parent, TBNode* node)
{
    auto ret = new Widget();
   
    INFLATE_INFO info( g_widgets_reader, parent, node, TBValue::TYPE_NULL );

    // this adds it to the parent, see 'TBWidget::OnInflate()' 
    ret->OnInflate( info );

    // now read and add children
    g_widgets_reader->LoadNodeTree( ret, node );

    return ret;
}

template <typename Widget>
Widget* new_widget(TBNode* node)
{
    // ad hoc
    auto* parent = new TBWidget();
    Widget* ret = new_widget<Widget>( parent, node );
    parent->RemoveChild( ret );

    return ret;
}


} // namespace tb


#endif

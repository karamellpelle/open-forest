//    open-forest: an orienteering game.
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
#include "batb/tb.hpp"
#include "tb_linklist.h"
#include "tb_widgets.h"
#include "tb/tb_widgets_reader.h"
#include "tb/tb_window.h"


namespace tb
{

// TODO: remove this widget!!
// copied from TBEditFieldEnter:
TB_WIDGET_FACTORY(TBEditFieldEnter, TBValue::TYPE_STRING, WIDGET_Z_TOP) {}
//void TBEditFieldEnter::OnInflate(const INFLATE_INFO &info)
//{
//
//}

// (we keep OnInflate from TBEditField; our subclass only overrides one function)

////////////////////////////////////////////////////////////////////////////////
// TBWindow. not present in turbobadger source :/
//
TB_WIDGET_FACTORY(TBWindow, TBValue::TYPE_STRING, WIDGET_Z_TOP) {}
//void TBWindow::OnInflate(const INFLATE_INFO &info)
//{
//    // TODO!
///*
//	if (const char *text_align = info.node->GetValueString("text-align", nullptr))
//	{
//		if (!strcmp(text_align, "left"))		SetTextAlign(TB_TEXT_ALIGN_LEFT);
//		else if (!strcmp(text_align, "center"))	SetTextAlign(TB_TEXT_ALIGN_CENTER);
//		else if (!strcmp(text_align, "right"))	SetTextAlign(TB_TEXT_ALIGN_RIGHT);
//	}
//	TBWidget::OnInflate(info);
//	if (const char *filename = info.node->GetValueString("filename", nullptr))
//		SetImage(filename);
//
//	if (TBNode *items = node->GetNode("items"))
//	{
//		for (TBNode *n = items->GetFirstChild(); n; n = n->GetNext())
//		{
//			if (strcmp(n->GetName(), "item") != 0)
//				continue;
//			const char *item_str = n->GetValueString("text", "");
//			TBID item_id;
//			if (TBNode *n_id = n->GetNode("id"))
//				TBWidgetsReader::SetIDFromNode(item_id, n_id);
//
//			TBGenericStringItem *item = new TBGenericStringItem(item_str, item_id);
//			if (!item || !target_source->AddItem(item))
//			{
//				// Out of memory
//				delete item;
//				break;
//			}
//		}
//	}
//*/
//    TBWidget::OnInflate(info);
//}
//
//}
} // namespace tb




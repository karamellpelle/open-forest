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
#ifndef BATB_GUI_TB_TBPROGRESSBARWIDGET_HPP
#define BATB_GUI_TB_TBPROGRESSBARWIDGET_HPP
#include "include.hpp"
#include "tb/tb_widgets_common.h"


namespace tb
{


class TBProgressBarWidget : public TBButton
{
public:
    TBProgressBarWidget();

    // TODO: SetValue
    void SetAlpha(double a); // -> SetValue( a * 100.0 );
    //void SetText(const TBStr& );
    void Set(double a, const TBStr& s);

protected:
    //double m_alpha; 
    //TBStr m_text;
     
};



} // namespace tb


#endif


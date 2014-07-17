//    orienteering-game: an orientering game.
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
#ifndef BATB_GUI_TB_SYSTEM_HPP
#define BATB_GUI_TB_SYSTEM_HPP
#include "batb/batb_include.hpp"


namespace batb
{

namespace gui
{


// set time for TBSystem:
void tbsystem_ms(double t);

// TODO: set DPI, ..., for our TBSystem implementation


} // namespace gui

} // namespace batb


// these are part of TB but not namespace tb, 
// for some odd reason...
void register_tbbf_font_renderer();
void register_stb_font_renderer();
void register_freetype_font_renderer(); 

#endif

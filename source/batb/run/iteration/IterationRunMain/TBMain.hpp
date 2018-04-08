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
#ifndef BATB_RUN_TBMAIN_HPP
#define BATB_RUN_TBMAIN_HPP
#include "batb/batb_include.hpp"
#include "batb/gui.hpp"

namespace batb
{
class BATB;


namespace run
{

class IterationRunMain;
class World;


// the "main menu" widget
class TBMain : public tb::TBWidget
{
public:
    TBOBJECT_SUBCLASS( TBMain, tb::TBWidget );

    TBMain(BATB* );

    void step(World& );

    virtual bool OnEvent(const tb::TBWidgetEvent& ev) override;


    BATB* batb;

};


} // namespace run

} // namespace batb


#endif



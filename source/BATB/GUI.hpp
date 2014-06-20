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
#ifndef BATB_GUI_HPP
#define BATB_GUI_HPP
#include "BATB/Config.hpp"
#include "tb/tb_renderer.h"

// export these to rest
#include "tb/tb_window.h"


namespace BATB
{


class GUIRoot : public tb::TBWidget
{
public:
    //GUIRoot::output(wth, hth); // ?
    // setSize(wth, hth)        // ?
    // create from file
    //g_widgets_reader->LoadFile(this, filename)
};


// class for GUI-part of BATB
class GUI
{
public:
    GUI() { }

    void create(xml::XMLElement* );
    void destroy();

    void output(uint wth, uint hth);
    void step();

    void keysCallback(/*Keys*/);

    GUIRoot* root() { return &root_; }

    //
private:
    tb::TBRenderer* tb_renderer_;
    GUIRoot root_;



};



// the only GUI, access
inline GUI* theGUI()
{
    static GUI ret_;
    return &ret_;
}




}

#endif


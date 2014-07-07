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
#ifndef BATB_BATB_HPP
#define BATB_BATB_HPP
#include "batb/batb_include.hpp"
#include "batb/log.hpp"
#include "batb/value.hpp"
#include "batb/xml.hpp"
#include "batb/keys.hpp"
#include "batb/gui.hpp"
//#include "batb/forest.hpp"
//#include "batb/race.hpp"
#include "batb/run.hpp"


namespace batb
{


class BATB
{
friend void begin(BATB& batb);
friend void end(BATB& batb);

public:
    BATB(const std::string& );

    // core part of BATB.
    // these are the parts of BATB fully initialized by 'void begin(BATB& )'.
    // we need a minimum part for 'iterationRunBegin' to work.
    log::Log log;
    xml::XML xml;
    value::Value value;
    keys::Keys keys;
    gui::GUI gui;

    // non-core part of BATB.
    // these are parts of BATB initialized later, by 'iterationRunBegin'.
    //forest::Forest forest;
    //race::Race race;
    run::Run run;

    // save this BATB object to its XML file
    void saveXML();

private:
    bool initialized_ = false;

    std::string filepath_;

};


// start BATB-object
void begin(BATB& batb);


// end BATB-object
void end(BATB& batb);


} // namespace batb

#endif

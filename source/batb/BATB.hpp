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
#ifndef BATB_BATB_HPP
#define BATB_BATB_HPP
#include "batb/batb_include.hpp"
#include "batb/log.hpp"
#include "batb/value.hpp"
#include "batb/keys.hpp"
#include "batb/gl.hpp"
#include "batb/gui.hpp"
#include "batb/ogre.hpp"
#include "batb/al.hpp"
#include "batb/forest.hpp"
#include "batb/demo.hpp"
//#include "batb/race.hpp"
#include "batb/run.hpp"
#include "Module.hpp"


namespace batb
{


class BATB : public Module
{
friend void begin(BATB& batb);
friend void end(BATB& batb);

public:
    BATB(env::Env& );

    // our environment
    env::Env& env;

    // core part of BATB.
    // these are the parts of BATB fully initialized by 'void begin(BATB& )'.
    // we need a minimum part for 'iterationRunBegin' to work.
    log::Log log;
    value::Value value;
    keys::Keys keys;
    gl::GL gl;
    gui::GUI gui;
    al::AL al;



    // non-core part of BATB.
    // these are parts of BATB initialized later, by 'iterationRunBegin'.
    ogre::OGRE ogre;
    run::Run run;
    forest::Forest forest;
    //race::Race race;
    demo::Demo demo;

};


// start BATB-object
void begin(BATB& batb);


// end BATB-object
void end(BATB& batb);


} // namespace batb

#endif

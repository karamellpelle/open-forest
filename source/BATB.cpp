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
#include "include.hpp"
#include "BATB.hpp"
#include "BATB/Keys.hpp"
#include "BATB/Values.hpp"

namespace BATB
{

static Config* theConfig_;

void begin(Config* cfg)
{
    // set BATB configuration-object
    theConfig_ = cfg;

    // get XML-objects
    Config::Block* block = cfg->block();

    // create log stream
    Log::create( /* F */ block );

    // create values, from file
    Values::create( /* F */ block );

    // create keys
    Keys::create( /* F */ block );

    // create RunData-resource
    RunData::create( /* F */ block );

    // create ForestData-resource
    ForestData::create( /* F */ block );
     
}

void end()
{
    // FIXME: save config


    ForestData::destroy();
    RunData::destroy();

}

Config* config()
{
    return theConfig_;
}


}


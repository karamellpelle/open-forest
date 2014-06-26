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
#ifndef BATB_RUN_HPP
#define BATB_RUN_HPP
#include "BATB/Run/RunWorld.hpp"
#include "BATB/Run/RunPrim.hpp"
#include "BATB/Run/RunSettings.hpp"
#include "BATB/Run/RunKeys.hpp"


namespace BATB
{


class Run
{
public:
    Run() : prim( &prim_ ), settings( &settings_ ), keys( &keys_ )
    {
    }

    void create(xml::XMLElement* );
    void destroy();

    // 
    RunPrim* const      prim;
    RunSettings* const  settings;
    RunKeys* const      keys;


private:
    RunPrim prim_;
    RunSettings settings_;
    RunKeys keys_;


};


// the only Run, access
inline Run* theRun()
{
    static Run ret;
    return &ret;
}

}


#endif


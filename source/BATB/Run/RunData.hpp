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
#ifndef BATB_RUNDATA_HPP
#define BATB_RUNDATA_HPP
#include "BATB/Config.hpp"
#include "BATB/Run/RunPrim.hpp"
#include "BATB/Run/RunSettings.hpp"


namespace BATB
{


class RunData
{
public:
    static void create(xml::XMLElement* );
    static void destroy();
    static RunData* theRunData() { return &theRunData_; }

    // 
    RunPrim* prim;
    RunSettings* settings;


private:
    static RunData theRunData_;
    static RunPrim theRunPrim_;
    static RunSettings theRunSettings_;

    RunData() : prim( 0 ), settings( 0 )
    {

    }

};


}


#endif


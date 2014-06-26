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
#ifndef BATB_RACE_RACE_CLASS_ENTRY_HPP
#define BATB_RACE_RACE_CLASS_ENTRY_HPP
#include "BATB/Config.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Race/RacePunch.hpp"


namespace BATB
{

class RaceClassEntry
{
typedef std::vector<RacePunch> RacePunchList;

public:
    RaceClassEntry(Runner* r) : runner_( r )
    {
    }

    Runner* runner() const
    {
        return runner_;
    }
private:
    Runner* const runner_;
    
    RacePunchList punches_;

};

}

#endif

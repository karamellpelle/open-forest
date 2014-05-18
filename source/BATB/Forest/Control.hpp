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
#ifndef BATB_CONTROL_HPP
#define BATB_CONTROL_HPP


namespace BATB
{

class Control
{
    // Control implies a Terrain, where this control is placed.
    Terrain* terrain;

    uint code() const
    {
        return code;
    }
private:
    uint code_;
    
    // stats, like number of punches, Player's been here, etc.
    // ...

    // output props:
    // GLAnimation anim_; // regular, start point
};


}


#endif

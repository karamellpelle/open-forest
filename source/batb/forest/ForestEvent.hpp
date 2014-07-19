//    open-forest: an orientering game.
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
#ifndef BATB_FOREST_EVENT_HUPP
#define BATB_FOREST_EVENT_HUPP


namespace BATB
{


// ControlPunch 
//    a Runner punched a Control. an Iteration may for example see
//    if this Control is goal, and take actions based on that, for
//    example add Runner/Player to a result list. or look at split times
//    and make status updates about 
class ForestEvent 
{
public:
    enum Type {  };
    
    Type type;

private:
};

class ForestEventList /*: public Game::EventList<ForestEvent> */
{

};


}

#endif


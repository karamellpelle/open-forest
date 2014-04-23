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
#include "BATB/Run/RunData.hpp"
#include "BATB/Log.hpp"


namespace BATB
{


RunData RunData::theRunData_;
RunPrim RunData::theRunPrim_;
RunSettings RunData::theRunSettings_;


void RunData::create()
{
    // FIXME: from file/config?
  
    RunData* this_ = &theRunData_;

    log << "RunData::create() " << std::endl;    
  
    // create primitives
    this_->prim = &theRunPrim_;
    this_->prim->iterationDataBegin =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationDataEnd   =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationIntro     =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationMain      =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationOutro     =  Game::NoIteration<RunWorld>::create();

    // create settings
    this_->settings = &theRunSettings_;
    //this_->settings->


}


void RunData::destroy()
{
    log << "RunData::destroy() " << std::endl;    

}

};

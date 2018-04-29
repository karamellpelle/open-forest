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
#ifndef BATB_HPP
#define BATB_HPP
#include "include.hpp"
#include "Module.hpp"
#include "BATB/Log.hpp"
#include "BATB/LogIndent.hpp"
#include "BATB/Event.hpp"



namespace batb
{

class Player;

// forward delcare
//namespace log    { class Log; };
namespace value  { class Value; };
namespace screen { class Screen; };
namespace time   { class Time; };
namespace keys   { class Keys; };
namespace gl     { class GL; };
namespace gui    { class GUI; };
namespace al     { class AL; };
namespace ogre   { class OGRE; };
namespace run    { class Run; };
namespace forest { class Forest; };
namespace demo   { class Demo; };
//class race::Race race;



// class BATB: work environment
class BATB : public Module
{
public:
    BATB();
    ~BATB();

    // setup object
    void begin(const std::string& );
    void end();

    using Module::init;

    // this sets up the non-core part of BATB. it can also be done 
    // outside class, i.e. a worker thread
    void beginNonCore();

    // create a new frame for an iteration
    void frameBegin();
    void frameEnd();

    // who is currently playing?
    Player* player();


    // log is always present
    std::unique_ptr<Log>              log;

    ////////////////////////////////////////////////////////////////////////////////
    // core part of BATB.
    // we need a minimum part of BATB to start running Iteration's: the core part.
    // core will be fully initialized by 'void BATB::begin(const YAML::Node& )'.
    std::unique_ptr<value::Value>     value;
    std::unique_ptr<screen::Screen>   screen;
    std::unique_ptr<time::Time>       time;
    std::unique_ptr<keys::Keys>       keys;
    std::unique_ptr<gl::GL>           gl;
    std::unique_ptr<gui::GUI>         gui;
    std::unique_ptr<al::AL>           al;



    ////////////////////////////////////////////////////////////////////////////////
    // non-core part of BATB.
    // this part is initialized later, for example by 'void BATB::beginNonCore()'
    std::unique_ptr<ogre::OGRE>       ogre;
    std::unique_ptr<run::Run>         run;
    std::unique_ptr<forest::Forest>   forest;
    //std::unique_ptr<race::Race>       race;
    std::unique_ptr<demo::Demo>       demo;

};


////////////////////////////////////////////////////////////////////////////////

// make sure we can use std::unique_ptr<Log> as std::ostream
template <typename T>
inline std::ostream& operator<<(std::unique_ptr<Log>& l, const T& t)
{
    return (*l) << t;
}

} // namespace batb


#endif

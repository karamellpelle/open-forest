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
#include <ostream>
#include "batb/batb_include.hpp"
#include "batb/Log.hpp"
#include "Module.hpp"


namespace batb
{


namespace value  { class Value; };
namespace keys   { class Keys; };
namespace gl     { class GL; };
namespace gui    { class GUI; };
namespace al     { class AL; };
namespace ogre   { class OGRE; };
namespace run    { class Run; };
namespace forest { class Forest; };
//class race::Race race;
namespace demo   { class Demo; };



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

    // 
    std::unique_ptr<Log>              log;

    // our environment
    // TODO: remove and merge into BATB
    std::unique_ptr<env::Env>         env;

    ////////////////////////////////////////////////////////////////////////////////
    // core part of BATB.
    // we need a minimum part of BATB to start running Iteration's: the core part.
    // core will be fully initialized by 'void begin(const YAML::Node& )'.
    std::unique_ptr<value::Value>     value;
    std::unique_ptr<keys::Keys>       keys;
    std::unique_ptr<gl::GL>           gl;
    std::unique_ptr<gui::GUI>         gui;
    std::unique_ptr<al::AL>           al;



    ////////////////////////////////////////////////////////////////////////////////
    // non-core part of BATB.
    // this part is initialized later, for example by 'void begin_noncore()'
    std::unique_ptr<ogre::OGRE>       ogre;
    std::unique_ptr<run::Run>         run;
    std::unique_ptr<forest::Forest>   forest;
    //std::unique_ptr<race::Race>       race;
    std::unique_ptr<demo::Demo>       demo;

};

// make sure we can use pointer (i.e. unique_ptr) to log as ostream
template <typename T>
inline std::ostream& operator<<(std::unique_ptr<Log>& l, const T& t)
{
    return l->operator<<( t );
}
inline std::ostream& operator<<(std::unique_ptr<Log>& l, const std::string& str)
{
    return operator<<( *l, str );
}

// I wasn't able to make these compile, hence I create helper functions!
//std::ostream& operator<<(std::unique_ptr<Log>& l, std::ios& (*pf)(std::ios&))
//{
//    return l->operator<<( pf );
//}
//std::ostream& operator<<(std::unique_ptr<Log>& l, std::ios_base& (*pf)(std::ios_base&))
//{
//    return l->operator<<( pf );
//}
} // namespace batb


#endif

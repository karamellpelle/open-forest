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
#ifndef BATB_RUN_KEYSET_HPP
#define BATB_RUN_KEYSET_HPP
#include "BATB/Keys/KeySet.hpp"
#include "BATB/Keys/Key.hpp"
#include "BATB/Keys/KeyAlpha.hpp"
#include "BATB/Keys/KeyClicker.hpp"
#include "BATB/Keys/KeyKeyboardButton.hpp"
#include "BATB/Keys/KeyMouseAxis.hpp"
#include "BATB/Keys/KeyMouseButton.hpp"
#include "BATB/Keys/KeyMouseScroll.hpp"
#include "BATB/Keys/KeyPointer.hpp"
#include "BATB/Keys/KeySet.hpp"

namespace batb
{


namespace run
{


class KeySet : public keys::KeySet
{
public:
    KeySet(BATB* b);

    // define 'this' from map defined by file
    void load(const YAML::Node& );
    void save(YAML::Node& );

    // set clean state
    void reset();

    /////////////////////////////////////////////////
    // keys for Run to use
    keys::KeyClicker* console = nullptr;
    keys::KeyClicker* escape  = nullptr;
    keys::KeyClicker* fullscreen  = nullptr;
    keys::KeyClicker* notify_ok  = nullptr;

};

} // namespace run

} // namespace batb


#endif

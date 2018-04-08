//    open-forest: an orienteering game.
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
#ifndef BATB_MODULE_BATB_HPP
#define BATB_MODULE_BATB_HPP
#include "batb/batb_include.hpp"
#include "Module.hpp"


namespace batb
{

class BATB;

class ModuleBATB : public Module
{
public:
    ModuleBATB(BATB* b) : Module(), batb( b ) { }

    BATB* batb = nullptr;

protected:
    using Module::yaml;
    using Module::filepath;
    using Module::init_empty;
    using Module::init_nonempty;
    using Module::init;
};

} // namespace batb


#endif


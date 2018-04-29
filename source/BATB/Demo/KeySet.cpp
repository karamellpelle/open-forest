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
#include "BATB/Demo/KeySet.hpp"
#include "helpers/yaml.hpp"


namespace batb
{


namespace demo
{


KeySet::KeySet(BATB* b) : keys::KeySet( b )
{

}

void KeySet::load(const YAML::Node& yaml)
{
    using namespace keys;

    // TODO: release current pointers back to Keys

    batb->log << "map-view-full:  ";
    map_view_full = create<KeyClicker>( createSafeKey( yaml > "map-view-full" ) );
    batb->log->endl();

}

void KeySet::save(YAML::Node& yaml)
{
    // TODO!

}

void KeySet::reset()
{
    map_view_full->reset();
}

} // namespace demo

} // namespace batb


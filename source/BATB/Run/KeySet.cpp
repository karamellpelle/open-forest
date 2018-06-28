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
#include "BATB/Run/KeySet.hpp"
#include "helpers/yaml.hpp"


namespace batb
{


namespace run
{

KeySet::KeySet(BATB* b) : keys::KeySet( b )
{

}

void KeySet::load(const YAML::Node& yaml)
{
    using namespace keys;

    // TODO: release current pointers back to Keys!

    batb->log << "console:    ";
    console = create<KeyClicker>( createSafeKey( yaml > "console" ) );
    batb->log->endl();

    batb->log << "escape:     ";
    escape = create<KeyClicker>( createSafeKey( yaml > "escape" ) );
    batb->log->endl();

    batb->log << "fullscreen: ";
    fullscreen = create<KeyClicker>( createSafeKey( yaml > "fullscreen" ) );
    batb->log->endl();

    batb->log << "notify-dismiss:  ";
    notify_dismiss = create<KeyClicker>( createSafeKey( yaml > "notify-dismiss" ) );
    batb->log->endl();



}

void KeySet::save(YAML::Node& yaml)
{

}

void KeySet::reset()
{
    console->reset();
    escape->reset();
}

} // namespace run

} // namespace batb


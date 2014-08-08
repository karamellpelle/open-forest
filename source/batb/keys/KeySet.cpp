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
#include "batb/keys/KeySet.hpp"

namespace batb
{

namespace keys
{


// create from definition in file
Key* KeySet::createKey(/*std::string filepath*/)
{
    // FIXME:
    return nullptr;
}

void KeySet::clear()
{
    for (Container::iterator i = keys_.begin(); i != keys_.end(); ++i)
    {
        delete *i;
    }
    keys_.clear();
}


// FIXME: rename Key::clear()
// Key::clear() for all keys:
void KeySet::reset()
{
    for (Container::iterator i = keys_.begin(); i != keys_.end(); ++i)
    {
        (*i)->clear();
    }
}

// FIXME: rename Key::update()
// update all keys, to be done at each frame:
void KeySet::step(tick_t t)
{
    for (Container::iterator i = keys_.begin(); i != keys_.end(); ++i)
    {
        (*i)->update( t );
    }
}

} // namespace keys

} // namespace batb

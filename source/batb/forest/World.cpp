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
#include "batb/forest.hpp"
#include "batb/run.hpp"


namespace batb
{

namespace forest
{


Control* World::pushControl(const glm::vec2& point, const ControlDefinition& def)
{
    Control c;
    c.definition = def;
    c.pos = glm::vec4( point[0], point[1], 0.0, 1.0 );

    controls.push_front( c );

    return &controls.front();

}

} // namespace forest

} // namespace batb




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
#ifndef BATB_INCLUDE_HPP
#define BATB_INCLUDE_HPP
#include "include.hpp"
#include <stdint.h>
#include <iostream>
#include <string>
#include <stdexcept>
//#define GLM_MESSAGES          // let glm print system messages
#define GLM_FORCE_RADIANS     // let glm angles be in radians
//#define GLM_FORCE_INLINE      // let glm be inlined
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Env.hpp"
#include "helpers.hpp"
#include "BATB/Values.hpp"

namespace BATB
{

typedef main::uint uint;


// float or double?
typedef main::float_t float_t;

namespace xml = tinyxml2;

typedef Env::tick_t tick_t;

}

#endif

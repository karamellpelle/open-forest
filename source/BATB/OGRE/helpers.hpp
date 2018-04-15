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
#ifndef BATB_OGRE_HELPERS_HPP
#define BATB_OGRE_HELPERS_HPP
#include "include.hpp"
#include "OgreMatrix3.h"
#include "OgreMatrix4.h"
#include "OgreQuaternion.h"


namespace batb
{


namespace ogre
{

////////////////////////////////////////////////////////////////////////////////
// quaternion

inline Ogre::Quaternion cast(const glm::quat& quat)
{
    return Ogre::Quaternion( quat.w, quat.x, quat.y, quat.z ); 
}


////////////////////////////////////////////////////////////////////////////////
// matrices
// NOTE: Ogre saves the data in an array[x][y] where y is row
//       glm saves columns in an array[] (the opposite)


inline Ogre::Matrix4 cast(const glm::mat4& mat)
{
    const auto& a = mat[0];
    const auto& b = mat[1];
    const auto& c = mat[2];
    const auto& d = mat[3];
    return Ogre::Matrix4( a.x, b.x, c.x, d.x,
                          a.y, b.y, c.y, d.y,
                          a.z, b.z, c.z, d.z,
                          a.w, b.w, c.w, d.w
                        );
}

inline Ogre::Matrix3 cast(const glm::mat3& mat)
{
    const auto& a = mat[0];
    const auto& b = mat[1];
    const auto& c = mat[2];
    return Ogre::Matrix3( a.x, b.x, c.x,
                          a.y, b.y, c.y,
                          a.z, b.z, c.z
                        );
}

inline Ogre::Matrix3 cast_(const glm::mat4& mat)
{
    const auto& a = mat[0];
    const auto& b = mat[1];
    const auto& c = mat[2];
    return Ogre::Matrix3( a.x, b.x, c.x,
                          a.y, b.y, c.y,
                          a.z, b.z, c.z
                        );
}



////////////////////////////////////////////////////////////////////////////////
// vectors

inline Ogre::Vector4 cast(const glm::vec4& vec)
{
    return Ogre::Vector4( vec.x, vec.y, vec.z, vec.w );
}

inline Ogre::Vector3 cast_(const glm::vec4& vec)
{
    return Ogre::Vector3( vec.x, vec.y, vec.z );
}

  
} // namespace ogre

} // namespace batb





#endif


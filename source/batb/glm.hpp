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
#ifndef BATB_GLM_HPP
#define BATB_GLM_HPP
#include "batb/batb_include.hpp"
#include <iomanip>

namespace batb
{



template <typename GLMVec>
bool inside(const GLMVec& p, typename GLMVec::value_type r, const GLMVec& px)
{
    auto d = px - p;
    return glm::dot(d, d) <= r * r;
}



inline std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
    constexpr int w = 6; 
    return os << std::setprecision( 1 ) << std::fixed << std::setw( w )
              << std::setw( w ) << mat[0].x << " " << std::setw( w ) << mat[1].x << " " << std::setw( w ) << mat[2].x << " " << std::setw( w ) << mat[3].x << "\n"
              << std::setw( w ) << mat[0].y << " " << std::setw( w ) << mat[1].y << " " << std::setw( w ) << mat[2].y << " " << std::setw( w ) << mat[3].y << "\n"
              << std::setw( w ) << mat[0].z << " " << std::setw( w ) << mat[1].z << " " << std::setw( w ) << mat[2].z << " " << std::setw( w ) << mat[3].z << "\n"
              << std::setw( w ) << mat[0].w << " " << std::setw( w ) << mat[1].w << " " << std::setw( w ) << mat[2].w << " " << std::setw( w ) << mat[3].w << "\n"
              ;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
    constexpr int w = 6; 
    return os << std::setprecision( 1 ) << std::fixed << std::setw( w )
              << "("
              << std::setw( w ) << vec.x
              << ", "
              << std::setw( w ) << vec.y
              << ")"
              ;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    constexpr int w = 6; 
    return os << std::setprecision( 1 ) << std::fixed << std::setw( w )
              << "("
              << std::setw( w ) << vec.x
              << ", "
              << std::setw( w ) << vec.y
              << ", "
              << std::setw( w ) << vec.z
              << ")"
              ;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
    constexpr int w = 6; 
    return os << std::setprecision( 1 ) << std::fixed << std::setw( w )
              << "("
              << std::setw( w ) << vec.x
              << ", "
              << std::setw( w ) << vec.y
              << ", "
              << std::setw( w ) << vec.z
              << ", "
              << std::setw( w ) << vec.w
              << ")"
              ;
}

inline glm::vec2 cast_xz(const glm::vec4& vec)
{
    return glm::vec2( vec.x, vec.z );
}

inline glm::vec2 cast_xz(const glm::vec3& vec)
{
    return glm::vec2( vec.x, vec.z );
}


} // namespace batb

#endif

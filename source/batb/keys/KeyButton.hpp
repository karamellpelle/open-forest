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
#ifndef BATB_KEYS_KEY_BUTTON_HPP
#define BATB_KEYS_KEY_BUTTON_HPP
#include "batb/keys/Key.hpp"
#include "batb/keys/Keys.hpp"

namespace batb
{


namespace keys
{

class KeyButton : public Key
{
public:
    KeyButton(Keys& keys, int c) : Key( typeid( this ), keys ), code_( c )    { } 

    void reset() override             { }
    void step(tick_t ) override       { }
    float_t alpha() override          { return keys_.getKey( code_ ) ? 1.0 : 0.0; } 


private:
    int code_;

};

// TODO: custom convert:
/*
namespace YAML {
template<>
struct convert<Vec3> {
  static Node encode(const Vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  static bool decode(const Node& node, Vec3& rhs) {
    if(!node.IsSequence() || node.size() != 3) {
      return false;
    }

    rhs.x = node[0].as<double>();
    rhs.y = node[1].as<double>();
    rhs.z = node[2].as<double>();
    return true;
  }
};
*/


} // namespace keys

} // namespace batb
#endif

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
#ifndef BATB_SCENE_HPP
#define BATB_SCENE_HPP
#include "BATB/Shape.hpp"

namespace batb
{



// defining a rendering target for OpenGL
// TODO: add Ogre::RenderTarget?
class Scene
{
public:
    Scene()             { }

    // the frambuffer 
    GLuint fbo          = 0;

    // size of scene, in fragments
    uint wth            = 0;
    uint hth            = 0;

    // physical size, in meters (if not known, use the same as Shape)
    float_t wth_m       = 0.0;
    float_t hth_m       = 0.0;

    // size of scene, in pixels. these may differ from fragments, for example on
    // Retina screens
    uint wth_px         = 0;
    uint hth_px         = 0;

    // shape of Scene
    Shape shape;

    // from 2D
    glm::mat4 proj2D;
    // from 3D
    glm::mat4 proj3D;

};







}

#endif

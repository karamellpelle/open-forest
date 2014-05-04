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
#ifndef BATB_SCENE_HPP
#define BATB_SCENE_HPP
#include "BATB/BATB_include.hpp"
#include "BATB/Shape.hpp"

namespace BATB
{


class Scene
{
public:
    Scene();

    void make();
    void remake(uint wth, uint hth);

    GLuint fbo; // custom framebuffer

    // pixel size
    uint wth;
    uint hth;

    // shape
    Shape shape;

    // projection matrices
    glm::mat4 proj2D;
    glm::mat4 proj3D;


};



inline Scene::Scene() : fbo( 0 ), wth( 0 ), hth( 0 ), shape( 0, 0 )
{
}


inline void Scene::make()
{
    // FIXME: create GL-stuff
}

inline void Scene::remake(uint wth, uint hth)
{
    // FIXME: destroy GL-stuff, create new GL-stuff

    // set Shape
    shape.wth = wth;
    shape.hth = hth;

    // GLM <3 <3 <3

    // set 2D-projection
    proj2D = glm::ortho( 0, shape.wth, shape.hth, 0 );

    // set 3D-projection
    proj3D = glm::perspective( valueProj3DFOVY, shape.wth / shape.hth, valueProj3DNear, valueProj3DFar );
}




// start scene for new frame
inline void scene_begin(Scene& scene)
{
    // handle size of screen changed
    uint wth;
    uint hth;
    Env::screenSize( wth, hth );
    if ( !( wth == scene.wth && hth == scene.hth ) )
    {
        scene.remake( wth, hth );
        
        glViewport( 0, 0, scene.wth, scene.hth );

    }

    // bind FBO to work on, i.e. screen
    //glBindFramebuffer( gl_FRAMEBUFFER, scene.fbo ); // FIXME
    // clear screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}
}

#endif

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
#include "BATB/Config.hpp"
#include "BATB/Shape.hpp"

namespace BATB
{


class Scene
{
public:
    Scene();

    // this creates new data, i.e. FBO
    void size(uint wth, uint hth);

    
    // we make sure these are const, in order to prevent mess 
    // up from something external
    GLuint            fbo() const         { return fbo_; }
    const Shape*      shape() const       { return &shape_; }
    const glm::mat4&  proj2D() const      { return proj2D_; }
    const glm::mat4&  proj3D() const      { return proj3D_; }

private:
    
    // framebuffer 
    GLuint fbo_;

    // size of FBO, pixels
    // not exposed, since projection matrices are only interesting
    uint wth_;
    uint hth_;

    // shape of FBO
    Shape shape_;

    // projection matrices
    glm::mat4 proj2D_;
    glm::mat4 proj3D_;


};



inline Scene::Scene() : fbo_( 0 ), wth_( 0 ), hth_( 0 ), shape_( 0, 0 )
{
}


inline void Scene::size(uint w, uint h)
{
    // FIXME:
    // destroy GL-stuff, if previously created
    // create new GL-stuff

    wth_ = w;
    hth_ = h;

    // set Shape
    shape_.size( wth_, hth_ );

    // GLM <3 <3 <3

    // set 2D-projection
    proj2D_ = glm::ortho( 0.0, shape_.wth, shape_.hth, 0.0 );

    // set 3D-projection
    proj3D_ = glm::perspective( valueProj3DFOVY, shape_.wth / shape_.hth, valueProj3DNear, valueProj3DFar );
}



inline Scene* theScene()
{
    static Scene ret;

    return &ret;
}


}

#endif

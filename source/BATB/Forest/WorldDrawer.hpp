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
#ifndef BATB_FOREST_WORLDDRAWER_HPP
#define BATB_FOREST_WORLDDRAWER_HPP
#include "include.hpp"

namespace Ogre { class RenderTarget; }

namespace batb
{
class BATB;

namespace forest
{
class World;





class WorldDrawer 
{
public:
    WorldDrawer();

    void init(World* );

    // draw to Ogre::RenderTarget
    void draw(Ogre::RenderTarget* );
#if 0
    // set view into World
    void cameraAim(Aim* , tick_t ticks = 0.0);
    void cameraFree();
    void cameraAround(Aim* );

    Aim aim_free;
    Aim aim_around;
#endif

private:
    World* forest_ = nullptr;
    Aim* aim_ = nullptr;

    
    Ogre::Camera* ogre_camera_              = nullptr;
    Ogre::SceneNode* ogre_camera_scenenode_ = nullptr;


};


} // namespace forest

} // namespace batb


#endif


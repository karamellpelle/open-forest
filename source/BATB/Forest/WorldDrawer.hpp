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
#include "BATB/Forest/AimControllable.hpp"
#include "BATB/Forest/WorldDrawer/Camera.hpp"



namespace Ogre
{
class RenderTarget;
class SceneNode;
}

namespace batb
{
class BATB;

namespace forest
{
class World;





class WorldDrawer : public AimControllable
{
public:
    WorldDrawer();

    // connect to world
    void init(World* );

    // draw to Ogre::RenderTarget
    void draw3D(Ogre::RenderTarget* );
    // update state
    void step(tick_t );

    // where to look from
    void camera(Ogre::SceneNode* , tick_t = 0.0);
    void cameraFree();
    void camera3thPerson(Ogre::SceneNode* , tick_t = 0.0);

    // AimControllable: let Keys control the view into World
    void aiming(const AimControllable::Aiming& ) override;

    // get current Aim in 3D world
    Aim getAim() const;

private:
    World* forest_ = nullptr;
    Camera camera_;    
    tick_t tick_ = 0.0;

    Ogre::Camera* ogre_camera_              = nullptr;
    Ogre::SceneNode* ogre_camera_scenenode_ = nullptr;

/*
    Camera camera_
    {
        OgreSceneNode* ogre_scene_node;
        DTMovable move;
        // ^ updating 'move' sets ogre_scene_node too
    }

    OgreSceneNode* ogre_scene_node0_;
    OgreSceneNode* ogre_scene_node1_;
    tick_t tick_;
    tick_t ticks_; 
    // void update()
    //   auto alpha = ticks_ == 0.0 ? 1.0 : std::max( 1.0, (tick - tick_)/ ticks_ );
    //   if ( alpha_ == 1.0 )
    //      ogre_camera->node = ogre_scene_node1_;
    //   else
    //      ogre_camera->node->pos = smooth( ogre_scene_node0_, ogre_scene_node1_, alpha );
    //      ogre_camera->node->aim = smooth( ogre_scene_node0_, ogre_scene_node1_, alpha );
*/

};


} // namespace forest

} // namespace batb


#endif


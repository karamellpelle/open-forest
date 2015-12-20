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
#include <iomanip>
#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
#include "OgreSceneNode.h"
#include "batb/run/Player.hpp"
#include "batb/forest/Runner.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb.hpp"


namespace batb
{

namespace forest
{

Runner::Runner(World& w, run::Player* p) : forest( w ), player( p ) 
{ 

}

// create internal from definition
void Runner::reset(const YAML::Node& yaml)
{
    ////////////////////////////////////////////////////////////////////////////////
    // see: SinbadCharacterController.h
   
    // TODO:std::string name = player ? player->name : " ";
    static uint ix = 0;
    std::ostringstream name;
    name << "Runner" << ix++;

    ogre_entity = forest.ogre_scenemgr->createEntity( name.str(), "Sinbad.mesh" );
    auto* node = forest.ogre_scenemgr->getRootSceneNode()->createChildSceneNode();
    node->scale( 3, 3, 3 ); // FIXME
    node->attachObject( ogre_entity );
        //
        // this is very important due to the nature of the exported animations
        //bodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);
        //String animNames[] =
        //{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
        //"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};
        //
        //// populate our animation list
        //for (int i = 0; i < NUM_ANIMS; i++)
        //{
        //    mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
        //    mAnims[i]->setLoop(true);
        //    mFadingIn[i] = false;
        //    mFadingOut[i] = false;
        //}
        //
        //// start off in the idle state (top and bottom together)
        //setBaseAnimation(ANIM_IDLE_BASE);
        //setTopAnimation(ANIM_IDLE_TOP);
        //
        //// relax the hands since we're not holding anything
        //mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

}

  

void Runner::reset(const glm::vec2& xz)
{
    // set correct height from terrain
    float_t x = xz.x;
    float_t z = xz.y;
    float_t y = forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( 
                Ogre::Vector3( x, 0.0, z ) );

    // 
    reset( glm::vec3( x, y + 14, z ) ); // TMP

}

void Runner::reset(const glm::vec3& xyz)
{
    control0 = nullptr;
    // fatigue, etc.

    move.aim[3].x = xyz.x; 
    move.aim[3].y = xyz.y; 
    move.aim[3].z = xyz.z; 
    move.aim[3].w = 1.0; 
    trace.reset( TracePoint( forest.tick, glm::vec3( xyz.x, xyz.y, xyz.z ) ) );
}

void Runner::punch(Control* control1)
{
    if ( control1 )
    {
        control1->punch( this );
        control0 = control1;
    }
}


void Trace::push(const TracePoint& point1)
{
    points.push_back( point0 );
    point0 = point1;

    std::cout << "added TracePoint: " 
              << "( "
              << std::setprecision( 1 )
              << std::fixed
              << point0.x << ", "
              << point0.y << ", "
              << point0.z << ") "
              << std::endl;
}

} // namespace forest

} // namespace batb




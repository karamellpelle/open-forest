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
#include <iomanip>
#include "BATB/Player.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/Runner.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/OGRE/helpers.hpp"
#include "BATB/Value/Forest.hpp"
#include "BATB/glm.hpp"

#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
#include "OgreSceneNode.h"
#include "OgreSkeletonInstance.h"
#include "OgreAnimation.h"
#include "OgreAnimationState.h"



namespace batb
{

namespace forest
{


Runner::Runner(World& w, Player* p) : forest( w ), player( p ), animation( *this )
{ 

}

Runner::Runner(const Runner& r) : 
    forest( r.forest ),
    player( r.player ), 
    map( r.map ), 
    move( r.move ),
    control0( r.control0 ), 
    trace( r.trace ),
    intensity( r.intensity ), 
    ogre_entity( r.ogre_entity ),
    animation( *this )
{

}

Runner::Runner(Runner&& r) :
    forest(                  r.forest   ), 
    player(       std::move( r.player ) ), 
    map(          std::move( r.map ) ),
    move(         std::move( r.move ) ),
    control0(     std::move( r.control0 ) ),
    trace(        std::move( r.trace ) ),
    intensity(    std::move( r.intensity ) ),
    ogre_entity(  std::move( r.ogre_entity ) ),
    animation( *this )
{

}





// create internal from definition
void Runner::reset(const YAML::Node& yaml)
{
    ////////////////////////////////////////////////////////////////////////////////
    // see: SinbadCharacterController.h
   
    std::ostringstream os( "Runner_" );
    if ( player == nullptr )
    {
        static uint ix = 0;
        os << ix++;
    }
    else
    {
        os << player->name;
    }

    ogre_entity = forest.ogre_scenemanager->createEntity( os.str(), "Sinbad.mesh" );
    ogre_entity->setCastShadows( true );

    ogre_scenenode = forest.ogre_scenemanager->getRootSceneNode()->createChildSceneNode();
    ogre_scenenode->scale( 3, 3, 3 ); // FIXME: use metric system all over (Terrain, Runners, Controls)
    ogre_scenenode->attachObject( ogre_entity );

    // setup animations for runner
    animation.reset( YAML::Node() /*yaml["animation"]*/ );

}

  

void Runner::reset(const glm::vec2& xz)
{
    // set correct height from terrain
    float_t x = xz.x;
    float_t z = xz.y;
    float_t y = forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( 
                Ogre::Vector3( x, 0.0, z ) );

    // 
    reset( glm::vec3( x, y + 15, z ) ); // TMP

}

void Runner::reset(const glm::vec3& xyz)
{
    control0 = nullptr;
    // fatigue, etc.

    move.pos.x = xyz.x; 
    move.pos.y = xyz.y; 
    move.pos.z = xyz.z; 
    move.pos.w = 1.0; 
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

void Runner::headlamp(bool on)
{
    using namespace Ogre;

    // create a headlamp if not attached
    if ( ogre_headlamp == nullptr )
    {

        ogre_headlamp = forest.ogre_scenemanager->createLight();
        ogre_headlamp->setType( Light::LT_SPOTLIGHT );
        ogre_headlamp->setSpotlightRange( Degree(20), Degree( 20 ));

        ogre_headlamp->setDiffuseColour( 1.0, 1.0, 1.0 );
        ogre_headlamp->setSpecularColour( 0.0, 0.0, 0.0 );

        // attach spotlight to runners head
        SceneNode* node = ogre_scenenode->createChildSceneNode(); // TODO: SceneNode of head
        node->setDirection( Vector3( 0, -1.0, 0 ).normalisedCopy() );
        node->setPosition(Vector3(0, 5, 0));
        node->attachObject( ogre_headlamp );

        //auto* entity = forest.ogre_scenemanager->createEntity( "ogrehead.mesh" );
        //SceneNode* entity_node = ogre_entity->getParentSceneNode()->createChildSceneNode(); // TODO: SceneNode of head
        //entity_node->setPosition(Vector3(0, 3, 0));
        //entity_node->setScale( 0.1, 0.1, 0.1 );
        //entity_node->attachObject( entity );
    }

    // turn headlamp on/off
    ogre_headlamp->setVisible( on );
}

void Runner::update()
{
    ////////////////////////////////////////////////////////////////////////////////
    // update physical state

    // set intensity
    intensity = std::abs( speed );

    // set velocity, based on Terrain, 
    // TODO: later use Terrain more active, like running slow in mosses, green parts of map, etc
    float_t incline = std::min( (float_t)( 0.95 ), forest.terrain.incline( move.aim ) ); // 0.95: prevent standing still
    float_t s = (1.0 - incline) * speed            // TODO: ensure 1.0 - 'incline' is OK
                            * value::forestModifyRunnerSpeed;

    // objects moves in z direction
    move.vel = (float)( s ) * move.aim[2]; 
 

    ////////////////////////////////////////////////////////////////////////////////
    // update Ogre state


    // update position of runner
    ogre_scenenode->setPosition( ogre::cast_( move.pos ) );

    // update orientation of runner
    auto quat = glm::quat_cast( move.aim );
    ogre_scenenode->setOrientation( ogre::cast( quat ) );


    // update animation
    animation.step( forest.tick );

}


// z is forward
// 'dir' gets normalized, so no need for unit vector
void Runner::setDirection(const glm::vec2& dir)
{
    auto u = glm::normalize( dir );


    // World direction in xz plane
    // NOTE: only update the rotation part, not translation (i.e. position)
    move.aim[0] = glm::vec4( u.y, 0.0, -u.x, 0.0 );
    move.aim[1] = glm::vec4( 0.0, 1.0, 0.0, 0.0 );
    move.aim[2] = glm::vec4( u.x, 0.0, u.y, 0.0 );
}

// 'speed' gets clamped to [-1, 1]
void Runner::setSpeed(float_t s)
{
    speed = clamp( s, (float_t)( -1.0 ), (float_t)( 1.0 ) );
}

////////////////////////////////////////////////////////////////////////////////
//


void RunnerAnimation::reset(const YAML::Node& yaml)
{
    // from SindbadCharacterController.h:
    runner.ogre_entity->getSkeleton()->setBlendMode( Ogre::ANIMBLEND_CUMULATIVE );

    ogre_animstate_base = runner.ogre_entity->getAnimationState( "RunBase" );
    ogre_animstate_top = runner.ogre_entity->getAnimationState( "RunTop" );
     
    ogre_animstate_base->setTimePosition( 0 );
    ogre_animstate_base->setLoop( true );
    ogre_animstate_base->setEnabled( true );
    ogre_animstate_top->setTimePosition( 0 );
    ogre_animstate_top->setLoop( true );
    ogre_animstate_top->setEnabled( true );

    //runner.ogre_entity->getAnimationState( "HandsRelaxed" )->setEnabled( true );
    runner.ogre_entity->getAnimationState( "HandsClosed" )->setEnabled( true );
    
    tick = 0.0;
}


void RunnerAnimation::step(tick_t tick_next)
{
    //TODO: prevent tick_next == tick

    auto delta = tick_next - tick;
    
    auto add = delta * std::max( value::forestRunnerAnimSpeedMin, runner.move.speed * value::forestRunnerAnimSpeed );
    ogre_animstate_base->addTime( add );
    ogre_animstate_top->addTime( add );

    tick = tick_next;
}

////////////////////////////////////////////////////////////////////////////////



} // namespace forest

} // namespace batb




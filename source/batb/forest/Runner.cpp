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
#include "OgreSkeletonInstance.h"
#include "OgreAnimation.h"
#include "OgreAnimationState.h"

#include "batb/run/Player.hpp"
#include "batb/forest/Runner.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/ogre/helpers.hpp"
#include "batb/value/forest.hpp"
#include "batb/glm.hpp"


namespace batb
{

namespace forest
{


Runner::Runner(World& w, run::Player* p) : forest( w ), player( p ), animation( *this )
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

    ogre_entity = forest.ogre_scenemgr->createEntity( os.str(), "Sinbad.mesh" );
    auto* node = forest.ogre_scenemgr->getRootSceneNode()->createChildSceneNode();
    node->scale( 3, 3, 3 ); // FIXME
    node->attachObject( ogre_entity );

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


void Runner::step()
{
    move.compute();
    auto node = ogre_entity->getParentNode();

    // update position of runner
    node->setPosition( ogre::cast_( move.pos ) );

    // update orientation of runner
    auto quat = glm::quat_cast( move.aim );
    node->setOrientation( ogre::cast( quat ) );

    // update animation
    animation.step( forest.tick );

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

void Trace::push(const TracePoint& point1)
{
    points.push_back( point0 );
    point0 = point1;

}


} // namespace forest

} // namespace batb




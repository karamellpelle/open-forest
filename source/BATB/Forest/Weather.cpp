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
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Terrain.hpp"


namespace batb
{

namespace forest
{


Weather::Weather(World* t) : forest( t )
{

}


void Weather::load(const YAML::Node& yaml)
{
    // TODO
    //
}

void Weather::setDay()
{
    using namespace Ogre;

    // fog
    forest->ogre_scenemanager->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000);
    
    // make sure we have a sun
    createSun();

    ogre_light_sun->setType( Light::LT_DIRECTIONAL );
    ogre_light_sun->setDirection( Vector3(0.55, -0.3, 0.75).normalisedCopy() );
    ogre_light_sun->setDiffuseColour( ColourValue::White );
    ogre_light_sun->setSpecularColour( ColourValue(0.6, 0.6, 0.6) );

    // ambient light
    //forest->ogre_scenemanager->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    forest->ogre_scenemanager->setAmbientLight(ColourValue(0.6, 0.6, 0.6));

    forest->terrain.ogre_terrain_globals->setLightMapDirection( ogre_light_sun->getDerivedDirection() );
    forest->terrain.ogre_terrain_globals->setCompositeMapAmbient( forest->ogre_scenemanager->getAmbientLight() );
    //forest->terrain.ogre_terrain_globals->setCompositeMapAmbient(ColourValue::Red);
    forest->terrain.ogre_terrain_globals->setCompositeMapDiffuse( ogre_light_sun->getDiffuseColour() );
    forest->terrain.ogre_terrain_globals->setLightMapDirection( ogre_light_sun->getDerivedDirection() );
    forest->terrain.ogre_terrain_globals->setCompositeMapAmbient( forest->ogre_scenemanager->getAmbientLight() );
    //forest->terrain.ogre_terrain_globals->setCompositeMapAmbient(ColourValue::Red);
    forest->terrain.ogre_terrain_globals->setCompositeMapDiffuse( ogre_light_sun->getDiffuseColour() );


    // sky box
    forest->ogre_scenemanager->setSkyBox( true, "Examples/CloudyNoonSkyBox" ); 
}

void Weather::setNight()
{
    using namespace Ogre;

    // fog
    forest->ogre_scenemanager->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000);

    // make sure we have a sun
    createSun();

    ogre_light_sun->setType( Light::LT_DIRECTIONAL );
    ogre_light_sun->setVisible( false );

    ogre_light_sun->setDirection( Vector3( 0.0, -1.0, 0.0 ) );
    ogre_light_sun->setDiffuseColour( ColourValue::White );
    ogre_light_sun->setSpecularColour( ColourValue(0.6, 0.6, 0.6) );

    // ambient light
    forest->ogre_scenemanager->setAmbientLight(ColourValue(0.14, 0.14, 0.14) );

    forest->terrain.ogre_terrain_globals->setLightMapDirection( ogre_light_sun->getDerivedDirection() );
    forest->terrain.ogre_terrain_globals->setCompositeMapAmbient( ColourValue(0,0,0) );
    //forest->terrain.ogre_terrain_globals->setCompositeMapAmbient(ColourValue::Red);
    forest->terrain.ogre_terrain_globals->setCompositeMapDiffuse( ColourValue(0,0,0) );
    forest->terrain.ogre_terrain_globals->setLightMapDirection( ogre_light_sun->getDerivedDirection() );
    //forest->terrain.ogre_terrain_globals->setCompositeMapAmbient(ColourValue::Red);

    // sky box
    //forest->ogre_scenemanager->setSkyBox( true, "Examples/CloudyNoonSkyBox" ); 
}


void Weather::setHourOfDay(float_t hour)
{
    // normalise into day
    hour = std::fmod( hour,  24.0 );

    // TODO: sun position and light based on hour an location on planet
}


void Weather::createSun()
{
    if ( ogre_light_sun == nullptr )
    {
        ogre_light_sun = forest->ogre_scenemanager->createLight( "ogre_light_sun" );
    }


}

} // namespace forest

} // namespace batb




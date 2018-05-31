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
#ifndef BATB_FOREST_WEATHER_HPP
#define BATB_FOREST_WEATHER_HPP
#include "include.hpp"

namespace Ogre
{
class Light;
}

namespace batb
{


namespace forest
{

class World;

// night/day and other properties
class Weather
{
public:
    Weather(World* t);

    void load(const YAML::Node& );

    // 24 times clock
    void setHourOfDay(float_t );

    // simple for now:
    void setDay();
    void setNight();

    //void setPercipitaion(/* none | rain | snow | hail | etc */);
    //void setFog(float_t 0.0..1.0);

    // TODO: terrain location on planet and date of year (for location and intensity)
    // ...
    // float_t speed; // forest.tick -> hour of day

    World* forest = nullptr;

    Ogre::Light*        ogre_light_sun = nullptr; // ->setType(Light::LT_DIRECTIONAL); see https://ogrecave.github.io/ogre/api/1.11/tut__lights_cameras_shadows.html

private:
    void createSun();


};




} // namespace forest

} // namespace batb



#endif

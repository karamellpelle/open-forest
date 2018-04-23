//    open-demo: an orientering game.
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
#include "BATB/Demo/World.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Run/World.hpp"


namespace batb
{


namespace demo
{



World::World(run::World& r) : run( r )
{
    forest = std::make_unique<forest::World>( run );
    // ^FIXME: pointers, not references?

    // make sure Course works on our forest world
    course.forest( forest.get() );

};


// load the demo world
void World::begin()
{

      ////////////////////////////////////////////////////////////////////////////////
      // add a runner
      runner = forest->addRunner( run.player ); 
      runner->reset( glm::vec2( 0, 0 ) );


}

#if 0
void World::createRandomCourse()
{
    using ControlDefinition = forest::ControlDefinition;   
    auto& forest = *demo.forest;

    glm::vec4 p0;
    glm::vec4 p1;
    if ( 2 <= demo.course.size() )
    {
        auto size = demo.course.size();

        // continue previous course 
        p0 = demo.course[ size - 2 ]->aim.pos;
        p1 = demo.course[ size - 1 ]->aim.pos;
    }
    else
    {
        // start new course
        p0 = glm::vec4( 0.0, 0.0, -1.0, 1.0);
        p1 = glm::vec4( 0.0, 0.0, 0.0, 1.0);

    }

    // clear course
    // FIXME: no remove?
    for (uint i = 0; i != demo.course.size(); ++i)
    {
        forest.removeControl( demo.course[i] );
    }
    demo.course.clear();


    ControlDefinition::Code code = 0;

    // Start
    demo.course.addControl( p1.x, p1.z, code++, ControlDefinition::Type::Start );

    static std::default_random_engine rand( std::chrono::system_clock::now().time_since_epoch().count() );

    constexpr uint max_controls = 16;
    uint m = std::uniform_int_distribution<uint>( 1, max_controls )( rand );

    for (uint i = 0; i != m; ++i)
    {
        glm::vec4 u = glm::normalize( p1 - p0 );
        auto trans = glm::mat4( u.x, 0.0, u.z, 0.0,
                                0.0, 0.0, 0.0, 0.0,
                                -u.z, 0.0, u.x, 0.0, 
                                p1.x, 0.0, p1.z, 1.0 );
     
        constexpr float_t spread_x0 = 300;
        constexpr float_t spread_x1 = 600;
        constexpr float_t spread_y = 1000;
        constexpr float_t s = 1.0;
        float_t x = s * std::uniform_real_distribution<float_t>( spread_x0, spread_x1 )( rand );
        float_t y = s * std::uniform_real_distribution<float_t>( -spread_y, spread_y )( rand );
        
        auto p2 = trans * glm::vec4( x, 0.0, y, 1.0 );

        if ( i + 1 == m )
        {
            // Finish
            code = std::max( (uint)(500), (uint)(code + 32) );
            demo.course.addControl( p2.x, p2.z, code, ControlDefinition::Type::Finish );
        }
        else
        {
            // random code number
            uint code_d = std::uniform_int_distribution<uint>( 1, 23 )( rand );
            code += code_d;
            
            // Normal
            demo.course.addControl( p2.x, p2.z, code, ControlDefinition::Type::Normal );
        }


        p0 = p1;
        p1 = p2;
    }

}
#endif

} // namespace demo

} // namespace batb





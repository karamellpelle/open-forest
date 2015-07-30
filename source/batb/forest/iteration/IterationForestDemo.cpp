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
#include "batb.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <random>
#include "helpers/bezier.hpp"

namespace batb
{

namespace forest
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationForestDemo::IterationForestDemo(BATB& b) : IterationForest( b )
{

}



void IterationForestDemo::iterate_begin(World& forest)
{
    BATB_LOG_FUNC( batb );

    // we want clean state for our Key's, no garbage:
    batb.forest.keyset.reset();

    // we want free movement (no pointer, only movement)
    //batb.keys.setCursorFree( true );

    if ( !forest.runners.empty() )
    {
        aiming = &forest.runners.front(); // is this pointer safe??

        // set start pos as in tmp::ogre:
        aiming->pos = glm::vec4( 0.0, 400.0, 0.0, 1.0 ); 
    }
    std::cout << "Starting Forest demo. " << std::endl;
}


IterationStack IterationForestDemo::iterate_forest(World& forest)
{
    ////////////////////////////////////////////////////////////////////////////////
    // output
    tmp_show_bezier( forest );

    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 

    float_t x, y;
    batb.forest.keyset.aim->axis( x, y );
    bool press_l = batb.forest.keyset.aim->left()->press();
    bool press_r = batb.forest.keyset.aim->right()->press();

    constexpr tick_t aim_dt = 0.02;
    aim_a = (-x) * 2.0;
    aim_b = y * 2.0;

    ////////////////////////////////////////
    // set aim of runner from aim_x_
    if ( aiming )
    {
        aiming->aim = glm::eulerAngleYXZ( aim_a, aim_b, aim_c );
    }


    ////////////////////////////////////////
    // * step dt of runner
    constexpr tick_t dt = 0.02;
    constexpr float_t press_speed = 100.0;

    tick_t tick_next = forest.run.tick;
    while ( forest.tick + dt <= tick_next )
    {
      
        // TODO: step 'aiming' (physics)
        if ( press_l || press_r )
        {
            glm::vec4 dir = aiming->aim[2];
            float alpha = ( press_r ? (-1.0) : (1.0) ) * dt * press_speed;
            aiming->pos += alpha * dir;
        }
        //
        forest.tick += dt;
    }

    glm::vec4 pos = aiming->pos;
    //std::cout << "\rcursor: " << x << " " << y << ", "
    //          << "pos: " << pos[0] << " "<< pos[1] << " "<< pos[2];
              

    // continue with this iteration
    return { this };

}


inline void nvg_point(NVGcontext* vg, uint x, uint y)
{
    constexpr float r = 4.0;
   
    
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x - r, y - r, 2 * r, 2 * r, 0.5 * r );
	nvgStroke(vg);
}

void IterationForestDemo::tmp_show_bezier(World& forest)
{
    // output
    gl::begin_nanovg();

    int winWidth, winHeight;
    glfwGetWindowSize( batb.env.window, &winWidth, &winHeight );
    int fbWidth, fbHeight;
    glfwGetFramebufferSize( batb.env.window, &fbWidth, &fbHeight );

    auto ctx = batb.gl.nvg_context;

    float pxRatio = (float)fbWidth / (float)winWidth; // Calculate pixel ration for hi-dpi devices.
    nvgBeginFrame( ctx, winWidth, winHeight, pxRatio, false ? NVG_PREMULTIPLIED_ALPHA : NVG_STRAIGHT_ALPHA);

    //float pxRatio  = 1.0;
    //nvgBeginFrame( ctx, fbWidth, fbHeight, pxRatio, false ? NVG_PREMULTIPLIED_ALPHA : NVG_STRAIGHT_ALPHA);

    nvgSave( ctx );
    //nvgScale( ctx, (float)(forest.run.scene.wth), (float)(forest.run.scene.hth) );

    nvgStrokeWidth( ctx, 4 );
    nvgLineCap(ctx, NVG_ROUND );
    nvgStrokeColor(ctx, nvgRGBA(0,192,255,128));
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, p0[0], p0[1] );
    nvgLineTo(ctx, p1[0], p1[1] );
    nvgStroke(ctx);

	nvgStrokeColor(ctx, nvgRGBA(0,200,0,200));
    nvgStrokeWidth( ctx, 2 );
    for (auto p : beziers_)
    {
    nvg_point( ctx, p[0], p[1] );
        
    }

    // draw bezier point 
    if ( !beziers_.empty() )
    {
        float t = 0.3 * forest.run.tick;
        float a = (float)( t - (uint)( t ) );
        glm::vec2 p = bezier( beziers_, a );
        nvgStrokeColor(ctx, nvgRGBA(200,0,0,200));
        nvg_point( ctx, p[0], p[1] );
    }
    
    nvgRestore( ctx );

    nvgEndFrame( ctx );
    gl::end_nanovg();

    // step
    if ( batb.forest.keyset.tmp->click() )
    {
        static std::default_random_engine rand; 

        
        std::uniform_int_distribution<uint> gen_x( 0, forest.run.scene.wth );
        std::uniform_int_distribution<uint> gen_y( 0, forest.run.scene.hth );
        //std::uniform_real_distribution<float> gen_x( 0.0, forest.run.scene.shape.wth );
        //std::uniform_real_distribution<float> gen_y( 0.0, forest.run.scene.shape.hth );

        
        p0 = glm::vec2( gen_x( rand ), gen_y( rand ) );
        p1 = glm::vec2( gen_x( rand ), gen_y( rand ) );

        std::cout << "p0: " << p0[0] << ", " << p0[1] << std::endl;
        std::cout << "p1: " << p1[0] << ", " << p1[1] << std::endl;
        beziers_ = random( p0, p1 );
    }

    gl::end_nanovg();

}
////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationForestDemo& iter)
{
    BATB_LOG_FUNC( iter.batb );


}

void end(IterationForestDemo& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

////////////////////////////////////////////////////////////////////////////////
// 
//

std::vector<glm::vec2> IterationForestDemo::random(const glm::vec2& p0, const glm::vec2& p1)
{
    using float_t = float;
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    constexpr uint n_between_max = 8;
    constexpr float_t r_step = 0.2;
    constexpr float_t r_step_normal = 0.5;

    static std::default_random_engine rand; // static necessary for "randomness"




    std::uniform_int_distribution<uint> gen_div( 0, n_between_max );
    uint n_between = gen_div( rand );
    uint n = 2 + n_between;

    float_t div = 1.0 / (float_t)( n );
  
    vec2 dir = p1 - p0; 
    vec2 dir_normal = vec2( -dir[1], dir[0] );

    std::vector<vec2> points( n );
    uint ix = 0;
    points[ix++] = p0;
    for (uint i = 0; i != n_between; ++i)
    {
        //std::uniform_real_distribution<float_t> gen_step( -r_step, r_step );
        //float_t step = ( 1.0 + (float_t)( i ) + gen_step( rand ) ) * div;
        std::uniform_real_distribution<float_t> gen_step( 0.0, 1.0 );
        float_t step = gen_step( rand );

        std::uniform_real_distribution<float_t> gen_step_normal( -r_step_normal, r_step_normal );
        float_t step_normal = gen_step_normal( rand );

        points[ix++] = glm::mix( p0, p1, step ) + step_normal * dir_normal; // TODO: glm smooth

    }
    points[ix++] = p1; 

    return points;
}


} // namespace forest

} // namespace batb



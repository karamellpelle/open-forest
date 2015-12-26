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
#include "batb/run/World.hpp"


namespace batb
{


namespace demo
{

static int image = -1;
static uint image_wth = 0;
static uint image_hth = 0;
static NVGpaint image_paint;


void other_begin(BATB& batb)
{
    auto* nvg = batb.gl.nvg_context;


    //glClearColor( 0, 0, 0.3, 0 );

    auto image_path = file::static_data( "batb/openforest-512x256.png" );
    image = nvgCreateImage( nvg, image_path.c_str(), 0 );
    int w, h;
    nvgImageSize( nvg, image, &w, &h );
    image_wth = w;
    image_hth = h;
   
}
    

void other_end(BATB& batb)
{
    auto* nvg = batb.gl.nvg_context;

    nvgDeleteImage( nvg, image );
    image = -1;
    image_wth = 0;
    image_hth = 0;
}

void background(BATB& batb, run::World& run)
{
    auto* nvg = batb.gl.nanovg_begin( run.scene );
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;

    //nvgTranslate( nvg, wth / 2, hth / 2 );
    
    float_t x = 0.5 * (wth - image_wth); 
    float_t y = 0.5 * (hth - image_hth); 
    image_paint = nvgImagePattern( nvg, x, y, image_wth, image_hth, 0.0, image, 1.0 );
    nvgBeginPath( nvg );
    //nvgRect( nvg, x, y, image_wth, image_hth );
    nvgRect( nvg, x, y, wth, hth );
    nvgFillPaint( nvg, image_paint );
    nvgFill( nvg );

    batb.gl.nanovg_end();
}


} // namespace demo


} // namespace batb





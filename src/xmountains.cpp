// This file is part of xmountains
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#include "crinkle.h"
#include "paint.h"
#include "global.h"
#include "patchlevel.h"
#include "copyright.h"

#include "matrix.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"

#define VERSION 2
#define SIDE 1.0

extern char *display;
extern char *geom;
int optind=1;
char *optarg;
int opterr=1;

Parm fold_param;
float vstrength; /* strength of vertical light source */
float lstrength; /* strength of vertical light source */

/*
 * extract the table of heights from the Strip struct
 * and discard the rest of the struct.
 */
Height* extract(Strip* s)
{
    int i;
  
    Height *p;
    p = s->d;
    free(s);
    for(i=0 ; i < width; i++)
    {
	p[i] = shift + (vscale * p[i]);
    }
    return(p);
}

void init_artist_variables()
{
    float dh, dd;
    int pwidth;  /* longest lengthscale for update */
  
    width= (1 << levels)+1;
    pwidth= (1 << (levels - stop))+1;

    /* make the fractal SIDE wide, this makes it easy to predict the
     * average height returned by calcalt. If we have stop != 0 then
     * make the largest update length = SIDE
     */
    cos_phi = cos( phi );
    sin_phi = sin( phi );
    tan_phi = tan( phi );

    x_fact = cos_phi* cos(alpha);
    y_fact = cos_phi* sin(alpha);
    vscale = stretch * pwidth;  /* have approx same height as fractal width
				 * this makes each pixel SIDE=1.0 wide.
				 * c.f. get_col
				 */

    /*
     * fdim conflicts with a definition in math.h,
     * consequently I have renamed fdim to f_dim
     * in the following locations:
     *     global.c:float fdim = 0.65;
     *     xmountains.c:  varience = pow( SIDE ,(2.0 * fdim));
     *     xmountains.c:  fold_param.fdim=fdim;
     *     global.h:extern float fdim;
     * 10.7.2005 tmj
     */
    /* guess the average height of the fractal */
    varience = pow( SIDE ,(2.0 * f_dim));
    varience = vscale * varience ;

    shift=0.5;

    shift = shift * varience;
    varience = varience + shift;

    start = (sealevel - shift) / vscale ; /* always start at sealevel */ 

    /* set the position of the view point */
    viewheight = altitude * width;
    viewpos = - distance1 * width;

    /* set viewing angle and focal length (vertical-magnification)
     * try mapping the bottom of the fractal to the bottom of the
     * screen. Try to get points in the middle of the fractal
     * to be 1 pixel high
     */
    dh = viewheight;
    dd = (width / 2.0) - viewpos;
    focal = sqrt( (dd*dd) + (dh*dh) );
#ifndef SLOPPY
    tan_vangle = (double) ((double)(viewheight-sealevel)/(double) - viewpos);
    vangle = atan ( tan_vangle );
    vangle -= atan( (double) (height/2) / focal ); 
#else
    /* we are making some horrible approximations to avoid trig funtions */
    tan_vangle = (double) ((double)(viewheight-sealevel)/(double) - viewpos);
    tan_vangle = tan_vangle - ( (double) (height/2) / focal );
#endif

    fold_param.mean=mean;
    fold_param.rg1=smooth & 4;
    fold_param.rg2=smooth & 2;
    fold_param.rg3=smooth & 1;
    fold_param.cross=cross;
    fold_param.force_front=slope;
    fold_param.force_back=0;
    fold_param.forceval=forceheight;
    fold_param.mix=mix;
    fold_param.midmix=midmix;

    /*
     * fdim conflicts with a definition in math.h,
     * consequently I have renamed fdim to f_dim
     * in the following locations:
     *     global.c:float fdim = 0.65;
     *     xmountains.c:  varience = pow( SIDE ,(2.0 * fdim));
     *     xmountains.c:  fold_param.fdim=fdim;
     *     global.h:extern float fdim;
     * 10.7.2005 tmj
     */
    fold_param.fdim = f_dim;
    top = make_fold(&fold_param, levels,stop,(SIDE / pwidth));

    a_strip = extract( next_strip(top) ); 
    b_strip = extract( next_strip(top) );

    /* initialise the light strengths */
    vstrength = vfract * contrast /( 1.0 + vfract );
    lstrength = contrast /( 1.0 + vfract );
}

void next_col(int paint, int reflec)
{
    free(a_strip);
    a_strip=b_strip;
    b_strip = extract( next_strip(top) );
}

int s_height=768, s_width=1024;
int mapwid;

void xmPlotColumn(TerrainProxy& terrainProxy, int p)
{
    next_col(0, 0);
    for(int j = 0; j < mapwid ; j++) 
    {
	terrainProxy.setData(p, j, b_strip[j]);
    }
}

void xmInit(TerrainProxy& terrainProxy)
{
    const Terrain* terrain = terrainProxy.getTerrain();

    terrainProxy.setHighest(FLT_MIN);
    terrainProxy.setLowest(FLT_MAX);
    s_width = terrain->width()+1;
    s_height = terrain->height()+1;
    seed = terrain->seed();
    stop = terrain->nonFractalIterations();
    levels = terrain->recursionLevels();
    f_dim = terrain->fractalDimension();
    
    height = s_height;
    seed_uni(seed);
    init_artist_variables();
    if( s_height > width ) 
    {
	mapwid = width;
    }
    else 
    {
	mapwid = s_height;
    }
}

void xmFinish()
{
    free(a_strip);
    free(b_strip);
    free_fold(top);
}

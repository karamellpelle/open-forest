#ifndef GLOBAL
#define GLOBAL

extern Fold *top;
extern int levels;
extern int stop;
extern int smooth;
extern int cross;
extern int slope;
extern int snooze_time;
extern int request_clear;
extern int n_col;
extern int band_size;
extern int e_events;
extern float mix;
extern float midmix;
/*
fdim conflicts with a definition in math.h,
consequently I have renamed fdim to f_dim
in the following locations:
global.c:float fdim = 0.65;
xmountains.c:  varience = pow( SIDE ,(2.0 * fdim));
xmountains.c:  fold_param.fdim=fdim;
global.h:extern float fdim;
10.7.2005 tmj
*/
extern float f_dim;
extern float start;
extern float mean;
extern float varience;
extern float shift;
extern float stretch;
extern float contour;
extern float ambient;
extern float contrast;
extern float vfract;
extern float altitude;
extern float distance1;
extern double phi;
extern double alpha;
extern double cos_phi;
extern double sin_phi;
extern double tan_phi;
extern double x_fact;
extern double y_fact;
extern Height sealevel;
extern Height forceheight;
extern int width;
extern int seed;
extern unsigned char red[256] ,green[256], blue[256];
extern int height;
extern double vangle;
extern double tan_vangle;
extern float vscale;
extern float viewpos;
extern float viewheight;
extern float focal;
extern Height *a_strip, *b_strip;

#endif

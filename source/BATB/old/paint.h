// This file is part of xmountains

#ifndef PAINT
#define PAINT

#include "crinkle.h"

/* colour code definitions */
typedef int Col;
typedef unsigned short Gun;


#define BLACK       0
#define WHITE       1
#define SEA_LIT     2
#define SEA_UNLIT   3
#define SKY         4
#define BAND_BASE   5
#ifndef BAND_SIZE
#define BAND_SIZE   40
#endif
#define N_BANDS     3
#define DEF_COL     (BAND_BASE + (N_BANDS * BAND_SIZE))
#define MIN_COL     (BAND_BASE + (N_BANDS * 2))
#define COL_RANGE   65535

#ifndef PI
#define PI 3.14159265
#endif

void set_clut(int max_col, Gun *red, Gun *green, Gun *blue);
Height* extract(Strip* s);
void init_artist_variables();
Col get_col(Height p, Height p_plus_x, Height p_plus_y, Height shadow);
Col *makemap(Height *a, Height *b, Height *shadow);
Col *camera(Height *a, Height *b, Height *shadow);
Col *mirror(Height *a, Height *b, Height *shadow);
int project( int x , Height y );
void finish_artist(void);
void flush_region(int x, int y, int w, int h);
void seed_uni(int ijkl);
float uni();

#endif

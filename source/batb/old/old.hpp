#ifndef BATB_OLD_OLD_HPP
#define BATB_OLD_OLD_HPP
#include "env.hpp"


namespace old
{

////////////////////////////////////////////////////////////////////////////////

void begin();

void iterate();

void end();

////////////////////////////////////////////////////////////////////////////////
// 

void reset_gl();

////////////////////////////////////////////////////////////////////////////////
//  handle old-errors (system exit)
//

void exit(int err);

bool is_exit();

////////////////////////////////////////////////////////////////////////////////

// we need these to port old-BATB into new-BATB
// the display functions drive the whole old-BATB...
typedef void (*DisplayFunc)();

void set_display_func(DisplayFunc );

DisplayFunc get_display_func();

////////////////////////////////////////////////////////////////////////////////
//
std::string file(const char* path);

int getElapsedTime();




} // namespace old



////////////////////////////////////////////////////////////////////////////////
// removing the GLUT-binding of plib that old-BATB used:
//
#include <plib/pu.h>

inline int puGetWindowOLD()
{
    //return glutGetWindow () ;
    return 0;
}

inline void puSetWindowOLD ( int window )
{
    //glutSetWindow ( window ) ; 
    return;
}

inline void puGetWindowSizeOLD ( int *width, int *height )
{
    //*width  = glutGet ( (GLenum) OLD_WINDOW_WIDTH  ) ;
    //*height = glutGet ( (GLenum) OLD_WINDOW_HEIGHT ) ;
    env::uint wth, hth;
    env::screen_size( wth, hth );

    *width = wth;
    *height = hth;
}

inline void puSetWindowSizeOLD ( int width, int height )
{
    //glutReshapeWindow ( width, height ) ;
    return;
}

inline void puInitOLD()
{
    puSetWindowFuncs ( puGetWindowOLD,
		       puSetWindowOLD,
		       puGetWindowSizeOLD,
		       puSetWindowSizeOLD ) ;
    puRealInit () ;
}




#endif

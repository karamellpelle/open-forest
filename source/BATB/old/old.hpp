#ifndef BATB_OLD_HPP
#define BATB_OLD_HPP
#include <iostream>
#include <string>
#include "BATB/BATB_include.hpp"
#include "BATB.hpp"

inline std::string old_file(const char* path)
{
    // executable is built into "build/" folder, point to data dir
    static std::string prefix = "data/old/";

    return prefix + path;
}

inline int oldGetElapsedTime()
{
    // assuming not too long elaps from Env::init to old start.
    Env::tick_t tick = Env::tick();
    return (int)( tick * 1000.0 );
}

namespace BATB
{

inline void exit(int err)
{
    extern bool old_exit;
    std::cout << "old exiting: " << err << std::endl;
    old_exit = true;
}

inline bool is_exit()
{
    extern bool old_exit;
    return old_exit;
    
}

}

////////////////////////////////////////////////////////////////////////////////
// removing GLUT-binding!
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
    Env::uint wth, hth;
    Env::screenSize( wth, hth );

    *width = wth;
    *height = hth;
}

inline void puSetWindowSizeOLD ( int width, int height )
{
    //glutReshapeWindow ( width, height ) ;
    return;
}

inline void puInitOLD ()
{
    puSetWindowFuncs ( puGetWindowOLD,
		       puSetWindowOLD,
		       puGetWindowSizeOLD,
		       puSetWindowSizeOLD ) ;
    puRealInit () ;
}

////////////////////////////////////////////////////////////////////////////////

void old_begin();

void old_end();


// we need these for display functions, since we are not using GLUT:
// also, the old-BATB display functions drive the whole game, i.e. iterates it.




// we need these to port old-BATB into new-BATB
// the display functions drive the whole old-BATB...
typedef void (*DisplayFunc)();
void set_display_func(DisplayFunc );
DisplayFunc get_display_func();

#endif

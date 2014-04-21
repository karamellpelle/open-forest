#ifndef ENV_SCREEN_HPP
#define ENV_SCREEN_HPP
#include "Env/include.hpp"

//#include <GL/glew.h>    // must be done before GLFW!!
#include <GLFW/glfw3.h> // FIXME: build option?


namespace Env
{


inline GLuint screenFBO()
{
    return 0;
}

inline void screenSize(uint& wth, uint& hth)
{
    wth = 0;
    hth = 0;
}

typedef double float_t;
inline void screenShape(float_t& wth, float_t& hth)
{
    uint wth_n; uint hth_n;
    screenSize( wth_n, hth_n );

    float_t wth_f = wth_n;
    float_t hth_f = hth_f;
    float_t scale = 1.0 / std::max( wth_n, hth_n );

    wth = wth_f * scale;
    hth = hth_f * scale;
}


}

#endif

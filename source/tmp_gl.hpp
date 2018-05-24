#ifndef TMP_GL_HPP
#define TMP_GL_HPP
#include "BATB/Screen.hpp"
#include "BATB.hpp"
#include "BATB/Time.hpp"

namespace batb
{

void gl_setup(BATB* batb);
void gl_shutdown(BATB* batb);
void gl_draw(BATB* batb);

} // namespace batb

#endif

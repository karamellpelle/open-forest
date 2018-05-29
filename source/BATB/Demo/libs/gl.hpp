#ifndef BATB_DEMO_LIBS_GL_HPP
#define BATB_DEMO_LIBS_GL_HPP

namespace batb
{

class BATB;
class Scene;

namespace demo
{

namespace gl
{


void tests_setup(BATB* batb);
void tests_shutdown(BATB* batb);
void tests_draw(const Scene&);
void tests_pause(bool );

} // namespace gl
} // namespace demo
} // namespace batb

#endif

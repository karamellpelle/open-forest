//    open-forest: an orienteering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
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

// linux
#ifdef BATB_BUILD_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX // or EGL??
#endif

// macOS
#ifdef BATB_BUILD_PLATFORM_MACOS
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#endif

#include <GLFW/glfw3native.h>
////////////////////////////////////////////////////////////////////////////////

#include "BATB/OGRE/GLContextGLFW.hpp"
#include "BATB/Screen.hpp"
#if 0
////////////////////////////////////////////////////////////////////////////////
namespace batb
{

namespace ogre
{

////////////////////////////////////////////////////////////////////////////////
//
void GLContextGLFW::begin()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    // see the line with _oneTimeContextInitialization();
    // inside void GLRenderSystem::_switchContext(GLContext *context)
    setInitialized();   
}


void GLContextGLFW::end()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

}

void GLContextGLFW::setCurrent()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    //
    // TODO: other platforms...
    // TODO: cant we just do glfwMakeContextCurrent( batb.env.window ) ??
#ifdef GLFW_EXPOSE_NATIVE_GLX
    ::Display* display = glfwGetX11Display();
    ::Window drawable = glfwGetX11Window( batb.env.window );
    ::GLXContext context = glfwGetGLXContext( batb.env.window );
    glXMakeCurrent( display, drawable, context );
#endif

    // apple
#ifdef BATB_BUILD_PLATFORM_MACOS
    glfwMakeContextCurrent( batb->screen->glfw_window );
#endif
    
}

void GLContextGLFW::endCurrent()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    // do nothing
}

Ogre::GLContext* GLContextGLFW::clone() const
{
    return nullptr;
}



} // namespace ogre

} // namespace batb

#endif


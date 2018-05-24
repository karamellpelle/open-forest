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
#include "BATB/Screen.hpp"
#include "BATB/Time.hpp"
#include "helpers/glfw.hpp"


extern "C"
{
static void glfw_error_callback(int error, const char* str)
{
    // FIXME: use 'batb' object!
    std::cout << "GLFW error (code " << error << "): " << str << std::endl;
}
}


namespace batb
{
namespace screen
{


////////////////////////////////////////////////////////////////////////////////
// setup

void Screen::begin(const std::string& path)
{
    batb->log << "batb->screen->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // setup GLFW
        //
        glfwSetErrorCallback( glfw_error_callback );

        if ( !glfwInit() )
        {
            batb->log << "ERROR: could not initialize GLFW" << std::endl;
            batb->log->indentPop(); 
            throw std::runtime_error( "Screen: Could not init GLFW" );
        }

        ////////////////////////////////////////////////////////////////////////////////
        // screen

        bool debugctx = yaml["debug"].as<bool>( false );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, debugctx );     // debug symbols (?) 
        batb->log << "debug context       = " << debugctx << std::endl;

        // multisamples
        uint samples = 0;
        if ( YAML::Node node = yaml[ "multisamples" ] )
        {
            samples = node.as<uint>( samples );
            glfwWindowHint( GLFW_SAMPLES, samples ); 
        }
        batb->log << "multisamples        = " << samples << std::endl;

        // size
        uint wth = 640;
        uint hth = 480;
        if ( YAML::Node node = yaml[ "size" ] )
        {
            wth = node[ "wth" ].as<uint>( wth );
            hth = node[ "hth" ].as<uint>( hth );
        }
        batb->log << "window size         = " << wth << "x" << hth << std::endl;


        // fullscreen
        bool fullscreen = false;
        if ( YAML::Node node = yaml[ "fullscreen" ] )
        {
            glfw_monitor = node.as<bool>( fullscreen ) ? glfwGetPrimaryMonitor() : 0;

            // if we create a fullscreen window, let us use the display's resolution
            if ( glfw_monitor )
            {
                if ( auto mode = glfwGetVideoMode( glfw_monitor ) )
                {
                    // save before we set fullscreen
                    nonfullscreen_wth_ = wth;
                    nonfullscreen_hth_ = hth;

                    wth = mode->width;
                    hth = mode->height;

                }
            }
        }
        batb->log << "window fullscreen   = " << fullscreen;
        if ( fullscreen ) batb->log << " (overriding window size with display resolution (" << wth << "x" << hth << ")";
        batb->log->endl();

        // title
        std::string title = "";
        if ( YAML::Node node = yaml[ "title" ] )
        {
            title = node.as<std::string>( title );
        }
        batb->log << "window title        = " << title << std::endl;

        // set addioninal windown hints
        // http://www.glfw.org/docs/latest/window.html#window_hints
        if ( YAML::Node node = yaml[ "glfw-hints" ] )
        {
            batb->log << "GLFW hints:" << std::endl;
            LogIndent indent( batb->log, "- " );
            
            for (auto p : node)
            {
                auto hint = p.first.as<std::string>();
                auto value = p.second.as<std::string>("");

                constexpr uint padding = 30;
                std::string pad( hint.size() < padding ? (padding - hint.size()) : 0, ' ' );

                batb->log << hint << pad << " = " << value;

                if ( glfw_set_windowhint( hint, value ) )
                {
                    batb->log->endl();
                }
                else
                {
                    batb->log << " (WARNING: could not set)" << std::endl;
                }
            }
        }
        
        // NOTE: error in implementation of glfw, according to valgrind:
        glfw_window = glfwCreateWindow( wth, hth, title.c_str(), glfw_monitor, 0 );

        // set GL context as 'theWindow_'
        glfwMakeContextCurrent( glfw_window );

        if ( !glfw_window )
        {
            batb->log << "ERROR: could not create GLFW window" << std::endl;
            batb->log->indentPop();
            throw std::runtime_error( "Screen: could not create GLFW window" );
        }

        // we now have a context, init GLEW
        // or other, see
        //  * http://www.glfw.org/docs/latest/context_guide.html#context_glext_auto
        //  * https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
        GLenum err = glewInit();
        if ( err != GLEW_OK )
        {
            batb->log << "ERROR: could not initialize the OpenGL loading library (GLEW): " << glewGetErrorString( err ) << std::endl;
            batb->log->indentPop();

            std::ostringstream os;
            os << "Screen: could not init GLEW (" << glewGetErrorString( err ) << ")";
            throw std::runtime_error( os.str() );
        }

        // print verbose GL info 
        if ( YAML::Node node = yaml[ "info" ] )
        {
            if ( node.as<bool>() )
            {
                printGLInfo();
            }
        }

static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
            glClearBufferfv(GL_COLOR, 0, red);

    }

    init( true );
}

void Screen::end()
{
    batb->log << "batb->screen->end()" << std::endl;
    LogIndent indent( batb->log, "-> " );
    if ( init_nonempty() )
    {
        ////////////////////////////////////////////////////////////////////////////////
        // TODO: write values from current settings into 'yaml' before saving
        save();

        ////////////////////////////////////////////////////////////////////////////////
        // screen
        glfwDestroyWindow( glfw_window ); 
        glfw_window = nullptr;
        glfw_monitor = nullptr;
        fbo_ = 0;
        fbo0_ = 0;
        fbo1_ = 0;

        ////////////////////////////////////////////////////////////////////////////////
        // frame
        frame_count_ = 0;
        frame_fps_ = 0.0;

        ////////////////////////////////////////////////////////////////////////////////
        // GLFW
        glfwTerminate();

    }

    init( false );
}

////////////////////////////////////////////////////////////////////////////////
// 

void Screen::printGLInfo()
{
    // see
    // https://www.khronos.org/files/opengl4-quick-reference-card.pdf
    // https://www.khronos.org/opengl/wiki/GLAPI/glGetString
    // https://www.khronos.org/registry/OpenGL-Refpages/
    // https://stackoverflow.com/questions/27407774/get-supported-glsl-versions
    // https://stackoverflow.com/questions/27407774/get-supported-glsl-versions
    batb->log << "GL vendor:    " << glGetString( GL_VENDOR ) << std::endl;
    batb->log << "GL version:   " << glGetString( GL_VERSION) << std::endl;
    batb->log << "GL renderer:  " << glGetString(GL_RENDERER) << std::endl;
    batb->log << "GL shader:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    batb->log << "GL extensions:" << std::endl;
    {
        LogIndent indent( batb->log, "- " );

        GLint num;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num );
        for ( GLint i = 0; i != num; ++i)
        {
            batb->log << glGetStringi(GL_EXTENSIONS, i ) << std::endl;

        }
    }
}
////////////////////////////////////////////////////////////////////////////////
// frame
//

void Screen::frameBegin()
{
    auto tick = batb->time->get();

    // compute frames per second
    constexpr tick_t update_delta = 0.4; // update interval in seconds
    double delta = tick - frame_fps_tick_;
    if ( update_delta <= delta )
    {
        frame_fps_ = (frame_count_ - frame_fps_count_) / delta;

        frame_fps_tick_ = tick; 
        frame_fps_count_ = frame_count_;
    }

    // populate events
    glfwPollEvents();
}

void Screen::frameEnd()
{
    glfwSwapBuffers( glfw_window );

    ++frame_count_;

}

////////////////////////////////////////////////////////////////////////////////
// screen
//
// TODO: helper FBO (fbo1), change size on window resize

void Screen::getSize(uint& wth, uint& hth) const
{
    int w, h;
    glfwGetFramebufferSize( glfw_window, &w, &h );
    wth = w;
    hth = h;
}

void Screen::getShape(float_t& wth, float_t& hth) const
{
    uint w, h;
    getSize( w, h );

    float_t scale = 1.0 / std::max( w, h );
    wth = (float_t)( w ) * scale;
    hth = (float_t)( h ) * scale;
}

GLuint Screen::setFBO(GLuint fbo)
{
    GLuint ret = fbo_;

    // TODO: bind 'fbo'
    batb->log << DEBUG_FUNCTION_NAME << ": TODO: FBO not implemented." << std::endl;
    fbo_ = fbo;

    return ret;
}

void Screen::fullscreen(bool full)
{

    if ( full )
    {
        batb->log << "batb->screen->fullscreen( true )" << std::endl;
        // save this before going fullscreen
        glfwGetWindowSize( glfw_window, &nonfullscreen_wth_, &nonfullscreen_hth_ );
        glfwGetWindowPos( glfw_window, &nonfullscreen_xpos_, &nonfullscreen_ypos_ );

        // full screen on
        glfw_monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode( glfw_monitor );
        glfwSetWindowMonitor( glfw_window,  glfw_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        batb->log << "batb->screen->fullscreen( false )" << std::endl;

        // back to window with previos pos and size
        glfw_monitor = nullptr;
        glfwSetWindowMonitor( glfw_window, glfw_monitor, nonfullscreen_xpos_, nonfullscreen_ypos_, nonfullscreen_wth_, nonfullscreen_hth_, 0 );
    }
}

bool Screen::closing()
{
    return glfwWindowShouldClose( glfw_window );
}

void Screen::closingClear()
{
    glfwSetWindowShouldClose( glfw_window, GLFW_FALSE);
}



} // namespace screen
} // namespace batb



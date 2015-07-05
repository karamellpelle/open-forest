//    open-forest: an orienteering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
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
#ifndef BATB_LOAD_THREAD_GL_CONTEXT_HPP
#define BATB_LOAD_THREAD_GL_CONTEXT_HPP
#include "batb/batb_include.hpp"


namespace batb
{


// move current GL context into background thread,
// new context will be bound to current thread. 
// 
// nice for loading resources in background thread
// while reporting progress on current thread.
//
// GL (GUI, nanovg) and GLFW (env) are allowed, so
// these can be used to report progress.
//
// Links:
// * https://www.opengl.org/wiki/OpenGL_Context
// * http://stackoverflow.com/questions/17779340/glfw-3-0-resource-loading-with-opengl
// * http://hacksoflife.blogspot.no/2008/02/creating-opengl-objects-in-second.html
// * http://stackoverflow.com/questions/3937257/is-opengl-threadsafe-for-multiple-threads-with-distinct-contexts
// 
// TODO: exception handling!
template<typename Current>
class ThreadGLContext
{
public:

    ThreadGLContext(const Current& cur = Current())
    {
        push_current( &cur ); 
    }
    /*virtual*/ ~ThreadGLContext() { }


    void begin(GLFWwindow* win = nullptr)
    {
        context_0_ = glfwGetCurrentContext();

        if ( win == nullptr )
        {
            // create new context
            // http://www.glfw.org/docs/latest/context.html#context_offscreen
            //
            // NOTE: only allowed on main thread (GLFW doc)
            glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
            win = glfwCreateWindow( 1, 1, "ThreadGLContext", nullptr, context_0_);

            free_ = win;
        }

        context_1_ = win;

        // run thread on context_0_ (set context virtual function 'void ThreadGLContext::run()')
        //thread_ = std::thread( &ThreadGLContext::run, std::ref( *this ) );
        thread_ = std::thread( &ThreadGLContext::begin_run, this );

        // switch to the new context_1_
        glfwMakeContextCurrent( context_1_ );

    }

    void end()
    {
        // TODO: join thread?
        thread_.join();

        // switch back context
        glfwMakeContextCurrent( context_0_ );


        // release context_1_ if it was temporary created
        glfwDestroyWindow( free_ );
        free_ = nullptr;
    }

    // nullptr iff complete!
    Current* current() { return current_; } // FIXME: return currents_.front() ?

    //std::forward_list<Current> list_currents()
    //{
    //    std::forward_list<Current> ret;

    //    {
    //        std::lock_guard<std::mutex> lock( mutex_ );
    //        ret = currents_; // copy
    //    }

    //    return ret;
    //} 

protected:
    virtual void run()
    {
    
        GLFWwindow * window = glfwGetCurrentContext();
        std::cout << __PRETTY_FUNCTION__ << "   ";
        std::cout << "thread::id: " <<  std::this_thread::get_id();
        std::cout << ", GLFWindow: " << window << std::endl;

    }

    void push_current(const Current* c)
    {
        if ( c == nullptr )
        {
            current_ = nullptr;             // nullptr, since std::atomic did not like const-cast
        }
        else
        {
            //std::lock_guard<std::mutex> lock( mutex_ );

            currents_.push_front( *c );     // NOTE: copy value
            current_ = &currents_.front();  // NOTE: point to value inside container, not 'c'
        }
    }


private:

    void begin_run()
    {
        // set the old context
        glfwMakeContextCurrent( context_0_ );

        // call subclass
        run();
     
    }

    // invariant: never popped, 
    std::forward_list<Current> currents_; 

    GLFWwindow* context_0_ = nullptr;
    GLFWwindow* context_1_ = nullptr;
    GLFWwindow* free_ = nullptr;

    std::thread thread_;
    std::atomic<Current*> current_;

};




} // namespace batb


#endif


//    open-forest: an orientering game.
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
#ifndef BATB_GUI_GUI_HPP
#define BATB_GUI_GUI_HPP
#include "batb/batb_include.hpp"

// TB
#include "tb/tb_renderer.h"
#include "tb/tb_window.h"
#include "tb/tb_font_renderer.h"


namespace batb
{

class BATB;
class Scene;
namespace keys { class Keys; }

namespace gui
{


class GUI
{
friend void begin(GUI& gui);
friend void end(GUI& gui);

public:
    GUI(BATB& b) : batb( b )
    {
    }

    // config file
    void filepath(const std::string& path)
    {
        filepath_ = path;
    }
    void save();

    // output GUI to env::screen
    void output(const Scene& );

    // step GUI
    void step(tick_t );

    // bind keys to this GUI object
    void bind(keys::Keys& );

    // root TB widget
    tb::TBWidget root;

    BATB& batb;

private:
    bool initialized_ = false;

    std::string filepath_;

    
    uint wth_ = 0;
    uint hth_ = 0;

    // TB
    tb::TBRenderer* tb_renderer_ = nullptr;
    
    // callbacks
    static tb::TBWidget* callback_widget;

    static void glfw_callback_char(GLFWwindow *window, unsigned int character);
    static void glfw_callback_key(GLFWwindow *window, int key, int scancode, int action, int glfwmod);
    static void glfw_callback_mouse_button(GLFWwindow *window, int button, int action, int glfwmod);
    static void glfw_callback_cursor_pos(GLFWwindow *window, double x, double y);
    static void glfw_callback_scroll(GLFWwindow *window, double x, double y);
    static void glfw_callback_window_size(GLFWwindow *window, int w, int h);


};

////////////////////////////////////////////////////////////////////////////////
//

void begin(GUI& );

void end(GUI& );


} // namespace gui


} // namespace batb

#endif


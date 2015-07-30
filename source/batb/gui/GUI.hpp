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
#include "batb/ModuleBATB.hpp"

// TB
#include "tb/tb_renderer.h"
#include "tb/tb_window.h"
#include "tb/tb_font_renderer.h"
#include "tb/tb_widgets.h"
#include "tb/tb_widgets_reader.h"
#include "tb/tb_widgets_common.h"
#include "tb/tb_node_tree.h"

namespace batb
{

class Scene;
namespace keys { class Keys; }

namespace gui
{


class GUI : public ModuleBATB
{
friend void begin(GUI& gui);
friend void end(GUI& gui);

public:
    GUI(BATB& b) : ModuleBATB( b )        { }

    // output GUI
    void output(const Scene& );

    // step GUI
    void step(tick_t );

    // bind keys to this GUI object
    void bind(keys::Keys& );

    // root TB widget
    tb::TBWidget root;

    void addWidget(tb::TBWidget* w)       { root.AddChild( w ); }
    void removeWidget(tb::TBWidget* w)    { root.RemoveChild( w ); }


private:
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


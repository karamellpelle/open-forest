//    orienteering-game: an orientering game.
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
#include "env/frame.hpp"

namespace env
{

extern GLFWwindow* screen_window_;
static uint count_ = 0;
static float_t fps_ = 0.0;
static float_t fps_delta_ = 1.0;

void frame_begin_()
{
    
}

void frame_end_()
{

}

void frame_begin()
{

}

void frame_end()
{
    glfwSwapBuffers( screen_window_ );
    glfwPollEvents();

    ++count_;

    static double tick_prev = 0.0;
    static uint count_prev = 0;

    double tick = glfwGetTime();
    if ( tick_prev + fps_delta_ <= tick )
    {
        fps_ = (count_ - count_prev) / fps_delta_;

        tick_prev = tick; 
        count_prev = count_;
    }
}

uint frame_count()
{
    return count_;
}

float_t frame_fps()
{
    return fps_;
}

}

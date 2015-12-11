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
#ifndef BATB_RUN_ITERATION_RUN_BEGIN_RUNWORKTBWIDGET_HPP
#define BATB_RUN_ITERATION_RUN_BEGIN_RUNWORKTBWIDGET_HPP
#include "batb/batb_include.hpp"
#include "tb/tb_layout.h"


namespace tb
{
    class TBProgressBarWidget;
    class TBImageWidget;
}


namespace batb
{

namespace run
{

class RunWorkTBWidget : public tb::TBLayout
{
public:
    RunWorkTBWidget(const std::string& = "" );
    ~RunWorkTBWidget();

    void set(float_t a, const std::string& msg = "");

private:

    tb::TBProgressBarWidget* tb_bar_ = nullptr;
    tb::TBImageWidget* tb_image_ = nullptr;
};


} // namespace run


} // namespace batb


#endif


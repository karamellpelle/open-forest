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
#include "batb.hpp"
#include "batb/run/iteration/IterationRunWork/RunWorkTBWidget.hpp"
#include "batb/gui/widgets/TBProgressBarWidget.hpp"
#include "tb/image/tb_image_widget.h"

namespace batb
{

namespace run
{

using namespace tb;

RunWorkTBWidget::RunWorkTBWidget(const std::string& path) : TBLayout( AXIS_Y )
{
/*
    // hardcode settings
    //
    SetSpacing( 0 );                                                        // space between children, override skin setting if not SPACING_FROM_SKIN
    SetLayoutSize( LAYOUT_SIZE_AVAILABLE );                                 // width for each child (AXIS_Y)
    SetLayoutPosition( LAYOUT_POSITION_CENTER );                            // x position for child (AXIS_Y)
    SetLayoutDistribution( LAYOUT_DISTRIBUTION_GRAVITY );                   // height for each child (AXIS_Y)
    SetLayoutDistributionPosition( LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP ); // y position for each child, if extra space (AXIS_Y)
    SetLayoutOverflow( LAYOUT_OVERFLOW_CLIP );                              // when this widget is too small for children

    ////////////////////////////////////////////////////////////////////////////////
    // add children:

    // image
    auto image_path = path.empty() ? file::static_data( "batb/openforest-512x256.png" ) : path;
    tb_image_ = new TBImageWidget();
    tb_image_->SetImage( image_path.c_str() );
    AddChild( tb_image_ );

    // progress bar
    tb_bar_ = new TBProgressBarWidget();
    tb_bar_->SetGravity( WIDGET_GRAVITY_LEFT_RIGHT );
    AddChild( tb_bar_ );

    // recompute
    InvalidateLayout(INVALIDATE_LAYOUT_RECURSIVE);
    Invalidate();

        //    SetSize( 360, 360  );
        //    SetPosition( TBPoint( 80, 80 ) );
        SetGravity( tb::WIDGET_GRAVITY_ALL );
	//ResizeToFitContent();
*/
}

RunWorkTBWidget::~RunWorkTBWidget()
{
    delete tb_bar_;
    delete tb_image_;
}


void RunWorkTBWidget::set(float_t alpha, const std::string& text)
{
    //tb_bar_->Set( alpha, text.c_str() );
    
}

} // namespace run


} // namespace batb




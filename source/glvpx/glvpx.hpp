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
#ifndef GLVPX_HPP
#define GLVPX_HPP
#include <stdint.h>
#include <ostringstream>
#include <stdexcept>


#define GLVPX_USE_VP8

namespace glvpx
{


// see libs/libvpx/examples/simpel_decoder.c

// == VP8 ==
#ifdef GLVPX_USE_VP8
  #define VPX_CODEC_DISABLE_COMPAT 1
  #include "vpx/vp8dx.h"
  #include "vpx/vpx_decoder.h"
#endif

typedef uint_fast32_t uint;


void begin();
void end();



class exception : public std::runtime_error
{


};



class file
{
friend frame* render_frame(file& f, double tick);
public:
    file(const char* );
    ~file();

    // FIXME: implement copy, assigment, ...

private:
    uint version_;
    uint frame_rate_;
    uint time_scale_;
    uint wth_;
    uint hth_;

    frame frame_;
    // 
    VpxVideoReader* reader_;
    VpxInterface* decoder_;
    vpx_codec_ctx_t codec_;
    
    // stats
    uint frames_;

};


//  object for frames (tex (+ audio, for WebM))
class frame
{
friend class file;
public:
    
    GLuint tex() const                    { return tex_; }
    void size(uint& wth, uint& hth) const
    {
        wth = wth_;
        hth = hth_;
    }
    double tick() const                   { return tick_; }

private:
    frame();

    GLuint tex_;

    // vars
    double tick_;
    uint wth_;
    uint hth_;


};


frame* const render_frame(file& f, double tick);


// default presenter, just render straight into screen.
void frame_present_std(const frame& f);





}


#endif

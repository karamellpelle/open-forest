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
#include "glvpx/glvpx.hpp"


// FIXME: disable-multithread!

namespace glvpx
{

typedef std::ostringstream ss;



static exception make_exception(ss& os)
{
    return exception( os.str() );
}



file::file(const char* path) : frames_( 0 ), version_( 0 ), frame_rate_( 0 ), time_scale_( 0 ),
                               wth_( 0 ), hth_( 0 ), reader_( 0 ), decoder_( 0 )
                                
{
    // create file reader
    reader_ = vpx_video_reader_open( path );
    if ( !reader_ )
    {
        throw make_execption( ss() << "could not open file '" << path << "'" );
    }

    // get info from file header 
    // reader.info does now contain the file header data
    //  - fourcc (VP80?)
    //  - width
    //  - height
    //  - time_base.numerator as frame rate
    //  - time_base.denumerator as time_scale
    VpxVideoInfo* info = vpx_video_reader_get_info( reader_ );
    if ( !info )
    {
        throw make_execption( ss() << "file is not vpx-file" );
    }

    // find decoder for this file, from reader.info.fourcc
    decoder_ = get_vpx_decoder_by_fourcc( info->codec_fourcc );
    if ( !decoder_ )
    {
        throw make_exception( ss() << "file is not vpx-file" );
    }

#ifdef GLVPX_MESSAGES
    std::cout << "using " << vpx_codec_iface_name( decoder->interface() ) << std::endl;
#endif

    // set size of frames
    wth_ = info->frame_width;
    hth_ = info->frame_height;
    // FIXME: get frame rate
    // FIXME: get time scale 
    // FIXME: number of frames ???

    // get our codec-context (codec_)
    if ( vpx_codec_dec_init( &codec_, decoder->interface(), NULL, 0) )
    {
        throw make_exception( ss() << "failed to initialize codec context" );
    }
   

    // create tex to render into
    glGenTextures( 1, &frame_.tex_ );
    glBindTexture( GL_TEXTURE_2D, tex );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_LINEAR ); 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_FILTER, GL_LINEAR );
  
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, wth_, hth_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );


  

}


file::~file()
{
      if ( reader_ )
      {
          vpx_video_reader_close( reader_ );
      }
      reader_ = 0;

      if ( codec_ )
      {
          if ( vpx_codec_destroy(&codec) )
          {
              throw make_exception( ss() << "could not destroy codec." );
          }
      }
      codec_ = 0;

}


frame* const render_frame(file& f, double tick)
{
    Frame* f = 0;
    while ( true )
    {
        f = get_next_frame();
        if ( f == 0 ) break;
        if ( f->time / fps <= tick ) break
    }
    if ( f == 0 )
    {
        return 0;
    }
    else
    {
        frame_.tex = glTexSubImage( f );
        frame_.time = f->time;

        return &frame;
    }
    // frame: 12 bytes header + data
    // reads header of frame
    //  - frame size without header
    //  - 
    // reads 'buffer' 'buffer_size
    // FIXME: returns timestamp?
    while ( vpx_video_reader_read_frame(reader_) )
    {
        reader->time_base
        
        // 
        size_t frame_size = 0;
        const uint8_t* frame = vpx_video_reader_get_frame( reader, &frame_size );

        if ( vpx_codec_decode( &codec_, frame, (unsigned int)frame_size, NULL, 0) )
        {
#ifdef GLVPX_MESSAGES
            std::cout << "could not decode frame." << std::endl;
#endif
            return true;
        }

        vpx_image_t* img = 0;
        vpx_codec_iter_t iter = 0;
        while ( (img = vpx_codec_get_frame( &codec, &iter )) != NULL )
        {
            // handle image
            // FIXME: set tick_
            // FIXME: set wth_, hth_, check correct?
            // FIXME: 
            glTexSubImage2D( );
            ++frames_;
        }
    }
  }

}


void frame_present_std(const frame& f)
{

}


}


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
#ifndef HELPERS_GL_HPP
#define HELPERS_GL_HPP
#include "include.hpp"


namespace gl
{

inline void error(std::ostream& os)
{
    GLenum err = glGetError();
    switch ( err )
    {
    case GL_INVALID_ENUM: os << "GL_INVALID_ENUM"; return;
    case GL_INVALID_VALUE: os << "GL_INVALID_VALUE"; return;
    case GL_INVALID_OPERATION: os << "GL_INVALID_OPERATION"; return;
    case GL_INVALID_FRAMEBUFFER_OPERATION: os << "GL_INVALID_FRAMEBUFFER_OPERATION"; return;
    case GL_INVALID_OUT_OF_MEMORY: os << "GL_INVALID_OUT_OF_MEMORY"; return;
    //case GL_INVALID_: os << "GL_INVALID_"; return;
    }

    os << "unknown( " << err << ")";
}

// makeGroupIBO
// uniformMat4
// uniformMat4AsMat3

// getUniformLocation
inline GLint get_uniform_location(GLint prg, const char* name)
{
    GLint loc = glGetUniformLocation( prg, name );
    if ( loc == -1 )
    {
        std::ostringstream os;
        os << "no such uniform location in program " << prg << ": " << name;
        throw std::runtime_error( os.str() );
    }

    return loc;
}


// uniformLocations
template <typename ArrayIn, typename ArrayOut>
inline void get_uniform_locations(GLint prg, const ArrayIn& names, std::size_t size, ArrayOut& locs)
{
    for (std::size_t i = 0; i != size; ++i)
    {
        locs[i] = get_uniform_location( prg, names[i] );
    }
}

// bindNewFBO. GL_READ_FRAMEBUFFER, GL_WRITE_FRAMEBUFFER
inline GLuint bind_new_fbo(GLenum target)
{
    GLuint ret;
    glGenFramebuffers( 1, &ret );
    glBindFramebuffer( target, ret );
    return ret;
}

// GL_RENDERBUFFER
inline GLuint bind_new_rbo(GLenum target)
{
    GLuint ret;
    glGenRenderbuffers( 1, &ret );
    glBindRenderbuffer( target, ret );
    return ret;
}

// GL_TEXTURE_1D , GL_TEXTURE_2D , GL_TEXTURE_1D_ARRAY , GL_TEXTURE_2D_ARRAY
// GL_TEXTURE_3D , GL_TEXTURE_RECTANGLE_BUFFER , GL_TEXTURE_CUBE_MAP
// GL_TEXTURE_CUBE_MAP_ARRAY , GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY
inline GLuint bind_new_tex(GLenum target)
{
    GLuint ret;
    glGenTextures( 1, &ret );
    glBindTexture( target, ret );
    return ret;
}

inline GLuint bind_new_vao()
{
    GLuint ret;
    glGenVertexArrays( 1, &ret );
    glBindVertexArray( ret );
    return ret;
}

// 
// GL_PIXEL_PACK_BUFFER , GL_PIXEL_UNPACK_BUFFER , GL_UNIFORM_BUFFER , GL_ARRAY_BUFFER
// GL_TEXTURE_BUFFER , GL_COPY_READ_BUFFER , GL_COPY_WRITE_BUFFER , GL_DRAW_INDIRECT_BUFFER
// GL_ELEMENT_ARRAY_BUFFER , GL_TRANSFORM_FEEDBACK_BUFFER , GL_ATOMIC_COUNTER_BUFFER
inline GLuint bind_new_buf(GLenum target)
{
    GLuint ret;
    glGenBuffers( 1, &ret );
    glBindBuffer( target, ret );
    return ret;
}

inline void del_fbo(GLuint name)
{
    glDel( 1, &name );
}
inline void del_rbo(GLuint name)
{
    glDelRendebuffers( 1, &name );
}
inline void del_tex(GLuint name)
{
    glDelTextures( 1, &name );
}
inline void del_buf(GLuint name)
{
    glDelBuffers( 1, &name );
}
inline void del_vao(GLuint name)
{
    glDelArrays( 1, &name );
}

//inline void create_prg(const char* vsh_path, const char* gsh_path, const char* fsh_path, ...)

//inline void create_ppo(const char* vsh_path, const char* gsh_path, const char* fsh_path, ...)

inline GLuint create_shader(GLenum type, const char* path)
{
    GLuint ret = glCreateShader( type );
    glShaderSource( ret, 0, 0, 0 ); // FIXME
    glCompileShader( ret );

    std::ostringstream os;
    GLint log_length;
    glGetShaderiv( shader GL_INFO_LOG_LENGTH, &log_length );
    if ( 0 < log_length )
    {
        const std::string& name = ""; // FIXME: filename of path
        GLchar log[ log_length ];
        glGetShaderInfoLog( ret, log_length, &log_length, log );

        os << "problems compiling shader " << ret << " (\"" << name << "\"): "; // FIXME: append log
        // correct to throw error?
        throw std::runtime_error( os.str() );
    }

    GLint status;
    glGetShaderiv( ret, GL_COMPILE_STATUS, &status );
    if ( status == 0 )
    {
        glDeleteShader( ret );
        throw std::runtime_error( "shader did not compile." );
    }

    return ret;
}


}

#endif

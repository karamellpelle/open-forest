// ================================================================================
// ==      This file is a part of Turbo Badger. (C) 2011-2014, Emil Segerås      ==
// ==                     See tb_core.h for more information.                    ==
// ================================================================================
// this file is taken from here and modified to our needs: https://github.com/tesch1/turbobadger/blob/50859e14bd8c923be65ccc8dfab237d30af19a23/src/tb/renderers/tb_renderer_gl.cpp
#include "BATB/GUI/tb/tb_renderer_gl_plus.h"
#include <fstream>

#include "tb_bitmap_fragment.h"
#include "tb_system.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace tb {

#ifdef TB_RUNTIME_DEBUG_INFO
uint32 dbg_bitmap_validations = 0;
#endif // TB_RUNTIME_DEBUG_INFO


static void MakeOrtho(float * ortho, float l, float r, float b, float t, float n, float f)
{
	ortho[0] = 2 / (r - l);
	ortho[1] = 0;
	ortho[2]  = 0;
	ortho[3] = 0;

	ortho[4] = 0;
	ortho[5] = 2 / (t - b);
	ortho[6]  = 0;
	ortho[7] = 0;

	ortho[8] = 0;
	ortho[9] = 0;
	ortho[10] = -2 / (f - n);
	ortho[11] = 0;

	ortho[12] = -(r+l)/(r-l);
	ortho[13] = -(t+b)/(t-b);
	ortho[14] = -(f+n)/(f-n);
	ortho[15] = 1;
}

// == Batching ====================================================================================

GLuint g_current_texture = (GLuint)-1;
TBRendererBatcher::Batch *g_current_batch = 0;

void BindBitmap(TBBitmap *bitmap)
{
//return;
	GLuint texture = bitmap ? static_cast<TBBitmapGLPlus*>(bitmap)->m_texture : 0;
	if (texture != g_current_texture)
	{
		g_current_texture = texture;

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, g_current_texture);
	}
}

// == TBBitmapGLPlus ==================================================================================

TBBitmapGLPlus::TBBitmapGLPlus(TBRendererGLPlus *renderer)
	: m_renderer(renderer), m_w(0), m_h(0), m_texture(0)
{
}

TBBitmapGLPlus::~TBBitmapGLPlus()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

	// Must flush and unbind before we delete the texture
	m_renderer->FlushBitmap(this);
	if (m_texture == g_current_texture)
		BindBitmap(nullptr);

	glDeleteTextures(1, &m_texture);
}

bool TBBitmapGLPlus::Init(int width, int height, uint32 *data)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

	assert(width == TBGetNearestPowerOfTwo(width));
	assert(height == TBGetNearestPowerOfTwo(height));

	m_w = width;
	m_h = height;

	glGenTextures(1, &m_texture);
	BindBitmap(this);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	SetData(data);

	return true;
}

void TBBitmapGLPlus::SetData(uint32 *data)
{

	m_renderer->FlushBitmap(this);
	BindBitmap(this);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	TB_IF_DEBUG_SETTING(RENDER_BATCHES, dbg_bitmap_validations++);
}

// == TBRendererGLPlus ================================================================================

TBRendererGLPlus::TBRendererGLPlus() : m_white(this)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

	GLchar vsh_cstr[] = R"(
        )";
	GLchar gsh_cstr[] = R"(
        )";
	GLchar fsh_cstr[] = R"(
        )";

        std::ifstream vsh_if( file::static_data( "tb.vsh" ) );
        std::string vsh_str( (std::istreambuf_iterator<char>(vsh_if) ), (std::istreambuf_iterator<char>()    ) );
        std::ifstream fsh_if( file::static_data( "tb.fsh" ) );
        std::string fsh_str( (std::istreambuf_iterator<char>(fsh_if) ), (std::istreambuf_iterator<char>()    ) );
        
        std::cout << vsh_str << fsh_str << std::endl;

        // create program from files
        m_program = make_program( "TBRendererGLPlus", vsh_str.c_str(), nullptr, fsh_str.c_str() );

        // retrieve uniforms
	m_orthoLoc = glGetUniformLocation(m_program, "ortho"); if ( m_orthoLoc == -1 ) std::cout << "WARNING: uniform 'ortho' does not exist" << std::endl;
	m_texLoc = glGetUniformLocation(m_program, "tex"); if ( m_texLoc == -1 ) std::cout << "WARNING: uniform 'tex' does not exist" << std::endl;

        // our VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate 1 buffer
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(batch.vertex), (void *)&batch.vertex[0], GL_DYNAMIC_DRAW);

	// Setup white 1-pixel "texture" as default
        uint32 whitepix = 0xffffffff;
        m_white.Init(1, 1, &whitepix);
}


void TBRendererGLPlus::BeginPaint(int render_target_w, int render_target_h)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );


#ifdef TB_RUNTIME_DEBUG_INFO
	dbg_bitmap_validations = 0;
#endif

	TBRendererBatcher::BeginPaint(render_target_w, render_target_h);

	g_current_texture = (GLuint)-1;
	g_current_batch = nullptr;

	glUseProgram(m_program);
	glBindVertexArray(m_vao);
	static float ortho[16];
	MakeOrtho(ortho, 0, (GLfloat)render_target_w, (GLfloat)render_target_h, 0, -1.0, 1.0);
	glUniformMatrix4fv(m_orthoLoc, 1, GL_FALSE, ortho);

	glViewport(0, 0, render_target_w, render_target_h);
	glScissor(0, 0, render_target_w, render_target_h);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TBRendererGLPlus::EndPaint()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

	TBRendererBatcher::EndPaint();

        // set back to default
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);

#ifdef TB_RUNTIME_DEBUG_INFO
	if (TB_DEBUG_SETTING(RENDER_BATCHES))
		TBDebugPrint("Frame caused %d bitmap validations.\n", dbg_bitmap_validations);
#endif // TB_RUNTIME_DEBUG_INFO
}

TBBitmap *TBRendererGLPlus::CreateBitmap(int width, int height, uint32 *data)
{

	TBBitmapGLPlus *bitmap = new TBBitmapGLPlus(this);
	if (!bitmap || !bitmap->Init(width, height, data))
	{
		delete bitmap;
		return nullptr;
	}
	return bitmap;
}

void TBRendererGLPlus::RenderBatch(Batch *batch)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

	// Bind texture and array pointers
#if defined(TB_RENDERER_GLES_2) || defined(TB_RENDERER_GL3)
	BindBitmap(batch->bitmap ? batch->bitmap : &m_white);
#else
	BindBitmap(batch->bitmap);
#endif

	if (g_current_batch != batch)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 2, GL_FLOAT,         GL_FALSE, sizeof(Vertex), &((Vertex *)nullptr)->x);
		glVertexAttribPointer(1, 2, GL_FLOAT,         GL_FALSE, sizeof(Vertex), &((Vertex *)nullptr)->u);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(Vertex), &((Vertex *)nullptr)->col);
		g_current_batch = batch;
	}

	// Flush
//#if defined(TB_RENDERER_GLES_2) || defined(TB_RENDERER_GL3)
	glBufferSubData(GL_ARRAY_BUFFER, 0, batch->vertex_count * sizeof(Vertex), (void *)&batch->vertex[0]);
//#endif
	glDrawArrays(GL_TRIANGLES, 0, batch->vertex_count);
}

void TBRendererGLPlus::SetClipRect(const TBRect &rect)
{

	glScissor(m_clip_rect.x, m_screen_rect.h - (m_clip_rect.y + m_clip_rect.h), m_clip_rect.w, m_clip_rect.h);
}

GLuint TBRendererGLPlus::make_shader(GLenum type, const GLchar* source)
{
    std::cout << "creating shader of type ";
    if ( type == GL_VERTEX_SHADER ) std::cout <<   "GL_VERTEX_SHADER  ";
    if ( type == GL_FRAGMENT_SHADER ) std::cout << "GL_FRAGMENT_SHADER";
    if ( type == GL_GEOMETRY_SHADER ) std::cout << "GL_GEOMETRY_SHADER";
    std::cout << ": ";

    GLuint ret = glCreateShader( type );
    glShaderSource( ret, 1, &source, NULL );
    glCompileShader( ret );

    GLint param;
    glGetShaderiv( ret, GL_COMPILE_STATUS, &param );
    if ( param == 0 ) std::cout << "ERROR: could not compile shader! ";


    // Get the log...
    GLint log_length;
    glGetShaderiv( ret, GL_INFO_LOG_LENGTH, &log_length);
    auto pstr = std::make_unique<GLchar[]>( log_length );
    
    glGetShaderInfoLog( ret, log_length, NULL, pstr.get());

    if ( log_length == 0 ) std::cout << "OK";
    else                   std::cout << std::endl << pstr.get();
    std::cout << std::endl;


    return ret;
}

GLuint TBRendererGLPlus::make_program(const GLchar* name, const GLchar* vsh_source, const GLchar* gsh_source, const GLchar* fsh_source)
{
    std::cout << "making program '" << name << "'" << std::endl;

    auto vsh = vsh_source ? make_shader( GL_VERTEX_SHADER, vsh_source ) : 0;
    auto gsh = gsh_source ? make_shader( GL_GEOMETRY_SHADER, gsh_source ) : 0;
    auto fsh = fsh_source ? make_shader( GL_FRAGMENT_SHADER, fsh_source ) : 0;

    // Create program, attach shaders to it, and link it
    GLuint ret = glCreateProgram();
    if ( vsh_source ) glAttachShader( ret, vsh);
    if ( gsh_source ) glAttachShader( ret, gsh );
    if ( fsh_source ) glAttachShader( ret, fsh );

    glLinkProgram( ret );

    GLint param;
    glGetProgramiv( ret, GL_LINK_STATUS, &param );
    if ( param == 0 ) std::cout << "ERROR: could not link program!" << std::endl;


    // Get the log...
    GLint log_length;
    glGetProgramiv( ret, GL_INFO_LOG_LENGTH, &log_length);
    auto pstr = std::make_unique<GLchar[]>( log_length );
    glGetProgramInfoLog( ret, log_length, NULL, pstr.get() );

    if ( log_length == 0 ) std::cout << "(program OK)";
    else                   std::cout << pstr.get();
    std::cout << std::endl;

    // Delete the shaders as the program has them now
    glDeleteShader( vsh );
    glDeleteShader( gsh );
    glDeleteShader( fsh );

    return ret; 
}



} // namespace tb


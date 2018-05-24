// ================================================================================
// ==      This file is a part of Turbo Badger. (C) 2011-2014, Emil Segerås      ==
// ==                     See tb_core.h for more information.                    ==
// ================================================================================
// karamellpelle: this file is taken from here: https://github.com/tesch1/turbobadger/blob/50859e14bd8c923be65ccc8dfab237d30af19a23/src/tb/renderers/tb_renderer_gl.h
//
#ifndef TB_RENDERER_GL_PLUS_H
#define TB_RENDERER_GL_PLUS_H

#include "BATB/Screen.hpp"

#include "tb_types.h"
#include "renderers/tb_renderer_batcher.h"


//#if defined(TB_RENDERER_GLES_1) && defined(TB_RENDERER_GLES_2)
//#error "Both GLES_1 and GLES_2 defined"
//#elif defined(TB_RENDERER_GLES_1) && defined(TB_RENDERER_GL3)
//#error "Both GLES_1 and GL3 defined"
//#elif defined(TB_RENDERER_GLES_2) && defined(TB_RENDERER_GL3)
//#error "Both GLES_2 and GL3 defined"
//#endif



namespace tb {

class TBRendererGLPlus;

class TBBitmapGLPlus : public TBBitmap
{
public:
	TBBitmapGLPlus(TBRendererGLPlus *renderer);
	~TBBitmapGLPlus();
	bool Init(int width, int height, uint32 *data);
	virtual int Width() { return m_w; }
	virtual int Height() { return m_h; }
	virtual void SetData(uint32 *data);
public:
	TBRendererGLPlus *m_renderer;
	int m_w, m_h;
	GLuint m_texture;
};

class TBRendererGLPlus : public TBRendererBatcher
{
public:
	TBRendererGLPlus();

	// == TBRenderer ====================================================================

	virtual void BeginPaint(int render_target_w, int render_target_h);
	virtual void EndPaint();

	virtual TBBitmap *CreateBitmap(int width, int height, uint32 *data);

	// == TBRendererBatcher ===============================================================

	virtual void RenderBatch(Batch *batch);
	virtual void SetClipRect(const TBRect &rect);


private:
        GLuint make_shader(GLenum type, const GLchar* source);
        GLuint make_program(const GLchar* name, const GLchar* vsh_source, const GLchar* gsh_source, const GLchar* fsh_source);

	GLuint m_program;
	GLuint m_vao;
	GLuint m_vbo;
	GLint m_orthoLoc;
	GLint m_texLoc;
	TBBitmapGLPlus m_white;

};

} // namespace tb

#endif // TB_RENDERER_GL_H

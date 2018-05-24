// karamellpelle: this file is based on this: https://github.com/tesch1/turbobadger/blob/50859e14bd8c923be65ccc8dfab237d30af19a23/src/tb/renderers/tb_renderer_gl.h
//
#ifndef TB_RENDERER_GL330_HPP
#define TB_RENDERER_GL330_HPP

#include "BATB/Screen.hpp"

#include "tb_types.h"
#include "renderers/tb_renderer_batcher.h"



namespace tb
{

class TBRendererGL330;

class TBBitmapGL330 : public TBBitmap
{
public:
	TBBitmapGL330(TBRendererGL330 *renderer);
	~TBBitmapGL330();
	bool Init(int width, int height, uint32 *data);
	virtual int Width() { return m_w; }
	virtual int Height() { return m_h; }
	virtual void SetData(uint32 *data);
public:
	TBRendererGL330 *m_renderer;
	int m_w, m_h;
	GLuint m_texture;
};



class TBRendererGL330 : public TBRendererBatcher
{
public:
	TBRendererGL330();

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
	TBBitmapGL330 m_white;

};

} // namespace tb

#endif // TB_RENDERER_GL_H

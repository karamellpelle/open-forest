#ifndef _TEXTURE_H
#define _TEXTURE_H


#include <GL/gl.h>


extern GLboolean
LoadRGBMipmaps(const char *imageFile, GLint intFormat);


extern GLboolean
LoadRGBMipmaps2(const char *imageFile, GLenum target,
		GLint intFormat, GLint *width, GLint *height);


extern GLubyte *
LoadRGBImage(const char *imageFile,
	     GLint *width, GLint *height, GLenum *format);

extern GLushort *
LoadYUVImage(const char *imageFile, GLint *width, GLint *height);

extern void loadTexture(GLuint texture, const char* filename);

#endif // _TEXTURE_H

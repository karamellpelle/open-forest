#ifndef _SHADER_H
#define _SHADER_H

int readShaderSource(const char *fileName, GLchar **vertexShader, GLchar **fragmentShader);
int installShaders(const GLchar *vertexShader, const GLchar *fragmentShader);

void setUniform1i(GLuint prog, const char* name, int value);
void setUniform1iv(GLuint prog, const char* name, int num, GLint* value);
void setUniform1f(GLuint prog, const char* name, float value);
void setUniform1fv(GLuint prog, const char* name, int num, GLfloat* value);
void setUniform2f(GLuint prog, const char* name, GLfloat x, GLfloat y);
void setUniform3f(GLuint prog, const char* name, GLfloat x, GLfloat y, GLfloat z);
void setUniform3fv(GLuint prog, const char* name, int num, GLfloat* value);
void setUniform4f(GLuint prog, const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void setUniformMatrix4fv(GLuint prog, const char* name, int num, float* value);

#endif // _SHADER_H

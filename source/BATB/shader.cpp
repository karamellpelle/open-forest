/* ===========================================================================
Copyright (C) 2002-2005  3Dlabs Inc. Ltd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
=========================================================================== */

#ifdef WIN32
#  include <windows.h>
//#  include <TIME.H> 
//#  include <io.h>
#else
#  include <unistd.h>
#endif

#include "ogl2brick.h"

//extern GLuint treeTextures[]; // in mainWindow.cxx
//extern GLuint groundTextures[]; // in mainWindow.cxx


//
// Global handles for the currently active program object, with its two shader objects
//
GLuint ProgramObject = 0;
GLuint VertexShaderObject = 0;
GLuint FragmentShaderObject = 0;


static int printOglError(const char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

//
// Get the location of a uniform variable
//
static GLint getUniLoc(GLuint program, const GLchar *name)
{
    GLint loc;

    loc = glGetUniformLocation(program, name);

    if (loc == -1)
        printf("No such uniform named \"%s\"\n", name);

    printOpenGLError();  // Check for OpenGL errors
    return loc;
}

//
// Print out the information log for a shader object
//
static void printShaderInfoLog(GLuint shader)
{
    int infologLength = 0;
    int charsWritten  = 0;
    GLchar *infoLog;

    printOpenGLError();  // Check for OpenGL errors

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

    printOpenGLError();  // Check for OpenGL errors

    if (infologLength > 0)
    {
        infoLog = (GLchar *)malloc(infologLength);
        if (infoLog == NULL)
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");
            exit(1);
        }
        glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
        printf("Shader InfoLog:\n%s\n\n", infoLog);
        free(infoLog);
    }
    printOpenGLError();  // Check for OpenGL errors
}


//
// Print out the information log for a program object
//
static void printProgramInfoLog(GLuint program)
{
    int infologLength = 0;
    int charsWritten  = 0;
    GLchar *infoLog;

    printOpenGLError();  // Check for OpenGL errors

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);

    printOpenGLError();  // Check for OpenGL errors

    if (infologLength > 0)
    {
        infoLog = (GLchar *)malloc(infologLength);
        if (infoLog == NULL)
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");
            exit(1);
        }
        glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
        printf("Program InfoLog:\n%s\n\n", infoLog);
        free(infoLog);
    }
    printOpenGLError();  // Check for OpenGL errors
}


static int shaderSize(const char *fileName, EShaderType shaderType)
{
    //
    // Returns the size in bytes of the shader fileName.
    // If an error occurred, it returns -1.
    //
    // File name convention:
    //
    // <fileName>.vert
    // <fileName>.frag
    //
    int fd;
    char name[100];
    int count = -1;

    strcpy(name, fileName);

    switch (shaderType)
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            printf("ERROR: unknown shader file type\n");
            exit(1);
            break;
    }

    //
    // Open the file, seek to the end to find its length
    //
#ifdef WIN32 /*[*/
    fd = _open(name, _O_RDONLY);
    if (fd != -1)
    {
        count = _lseek(fd, 0, SEEK_END) + 1;
        _close(fd);
    }
#else /*][*/
    fd = open(name, O_RDONLY);
    if (fd != -1)
    {
        count = lseek(fd, 0, SEEK_END) + 1;
        close(fd);
    }
#endif /*]*/

    return count;
}


static int readShader(const char *fileName, EShaderType shaderType, char *shaderText, int size)
{
    //
    // Reads a shader from the supplied file and returns the shader in the
    // arrays passed in. Returns 1 if successful, 0 if an error occurred.
    // The parameter size is an upper limit of the amount of bytes to read.
    // It is ok for it to be too big.
    //
    FILE *fh;
    char name[100];
    int count;

    strcpy(name, fileName);

    switch (shaderType) 
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            printf("ERROR: unknown shader file type\n");
            exit(1);
            break;
    }

    //
    // Open the file
    //
    fh = fopen(name, "r");
    if (!fh)
        return -1;

    //
    // Get the shader from a file.
    //
    fseek(fh, 0, SEEK_SET);
    count = (int) fread(shaderText, 1, size, fh);
    shaderText[count] = '\0';

    if (ferror(fh))
        count = 0;

    fclose(fh);
    return count;
}

int readShaderSource(const char *fileName, GLchar **vertexShader, GLchar **fragmentShader)
{
    int vSize, fSize;

    //
    // Allocate memory to hold the source of our shaders.
    //
    vSize = shaderSize(fileName, EVertexShader);
    fSize = shaderSize(fileName, EFragmentShader);

    if ((vSize == -1) || (fSize == -1))
    {
        printf("Cannot determine size of the shader %s\n", fileName);
        return 0;
    }

    *vertexShader = (GLchar *) malloc(vSize);
    *fragmentShader = (GLchar *) malloc(fSize);

    //
    // Read the source code
    //
    if (!readShader(fileName, EVertexShader, *vertexShader, vSize))
    {
        printf("Cannot read the file %s.vert\n", fileName);
        return 0;
    }

    if (!readShader(fileName, EFragmentShader, *fragmentShader, fSize))
    {
        printf("Cannot read the file %s.frag\n", fileName);
        return 0;
    }

    return 1;
}


int installShaders(const GLchar *vertexShader, const GLchar *fragmentShader)
{
    //GLuint brickVS, brickFS, brickProg;   // handles to objects
    GLuint brickVS, brickFS;   // handles to objects
    GLint  vertCompiled, fragCompiled;    // status values
    GLint  linked;

    // Create a vertex shader object and a fragment shader object

    brickVS = glCreateShader(GL_VERTEX_SHADER);
    brickFS = glCreateShader(GL_FRAGMENT_SHADER);

    // Load source code strings into shaders

    glShaderSource(brickVS, 1, &vertexShader, NULL);
    glShaderSource(brickFS, 1, &fragmentShader, NULL);

    // Compile the brick vertex shader, and print out
    // the compiler log file.

    glCompileShader(brickVS);
    printOpenGLError();  // Check for OpenGL errors
    glGetShaderiv(brickVS, GL_COMPILE_STATUS, &vertCompiled);
    printShaderInfoLog(brickVS);

    // Compile the brick vertex shader, and print out
    // the compiler log file.

    glCompileShader(brickFS);
    printOpenGLError();  // Check for OpenGL errors
    glGetShaderiv(brickFS, GL_COMPILE_STATUS, &fragCompiled);
    printShaderInfoLog(brickFS);

    if (!vertCompiled || !fragCompiled)
        return 0;

    // Create a program object and attach the two compiled shaders
	GLuint brickProg = glCreateProgram();
    glAttachShader(brickProg, brickVS);
    glAttachShader(brickProg, brickFS);

    // Link the program object and print out the info log

    glLinkProgram(brickProg);
    printOpenGLError();  // Check for OpenGL errors
    glGetProgramiv(brickProg, GL_LINK_STATUS, &linked);
    printProgramInfoLog(brickProg);

    if (!linked)
        return 0;

    return brickProg;
}

void setUniform1i(GLuint prog, const char* name, int value)
{
    glUseProgram(prog);
    glUniform1i(getUniLoc(prog, name), value);
    glUseProgram(0);
}

void setUniform1iv(GLuint prog, const char* name, int num, GLint* value)
{
    glUseProgram(prog);
    glUniform1iv(getUniLoc(prog, name), num, value);
    glUseProgram(0);
}

void setUniform1f(GLuint prog, const char* name, float value)
{
    glUseProgram(prog);
    glUniform1f(getUniLoc(prog, name), value);
    glUseProgram(0);
}

void setUniform1fv(GLuint prog, const char* name, int num, GLfloat* value)
{
    glUseProgram(prog);
    glUniform1fv(getUniLoc(prog, name), num, value);
    glUseProgram(0);
}

void setUniform2f(GLuint prog, const char* name, GLfloat x, GLfloat y)
{
    glUseProgram(prog);
    glUniform2f(getUniLoc(prog, name), x, y);
    glUseProgram(0);
}

void setUniform3f(GLuint prog, const char* name, GLfloat x, GLfloat y, GLfloat z)
{
    glUseProgram(prog);
    glUniform3f(getUniLoc(prog, name), x, y, z);
    glUseProgram(0);
}

void setUniform3fv(GLuint prog, const char* name, int num, GLfloat* value)
{
    glUseProgram(prog);
    glUniform3fv(getUniLoc(prog, name), num, value);
    glUseProgram(0);
}

void setUniform4f(GLuint prog, const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUseProgram(prog);
    glUniform4f(getUniLoc(prog, name), x, y, z, w);
    glUseProgram(0);
}

void setUniformMatrix4fv(GLuint prog, const char* name, int num, float* value)
{
    glUseProgram(prog);
    glUniformMatrix4fv(getUniLoc(prog, name), num, GL_FALSE, value);
    glUseProgram(0);
}

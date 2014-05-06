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

//
// The source of the shaders itself are in plain text files. Files ending in
// .vert are vertex shaders, and files ending in .frag are fragment shaders.
//

#include "ogl2brick.h"

//
// Handle of the window we're rendering to
//
static GLint window;

//
// Models to render
//
EModelType gleModel = EModelCube;

//
// Movement variables
//
float fXDiff = 206;
float fYDiff = 16;
float fZDiff = 10;
int xLastIncr = 0;
int yLastIncr = 0;
float fXInertia = -0.5;
float fYInertia = 0;
float fXInertiaOld;
float fYInertiaOld;
float fScale = 1.0;
float ftime = 0;
int xLast = -1;
int yLast = -1;
char bmModifiers;
int Rotate = 1;

//
// Rotation defines
//
#define INERTIA_THRESHOLD       1.0f
#define INERTIA_FACTOR          0.5f
#define SCALE_FACTOR            0.01f
#define SCALE_INCREMENT         0.5f
#define TIMER_FREQUENCY_MILLIS  20

GLfloat RotL = 1 * 3.14f / 180;
int LastTime = 0;

/*public*/
int printOglError(char *file, int line)
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


/***************************************************************************/
/* Parse GL_VERSION and return the major and minor numbers in the supplied
 * integers.
 * If it fails for any reason, major and minor will be set to 0.
 * Assumes a valid OpenGL context.
*/

void getGlVersion( int *major, int *minor )
{
    const char* verstr = (const char*)glGetString( GL_VERSION );
    if( (verstr == NULL) || (sscanf( verstr, "%d.%d", major, minor ) != 2) )
    {
        *major = *minor = 0;
        fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
    }
}


static void NextClearColor(void)
{
    static int color = 0;

    switch( color++ )
    {
        case 0:  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
             break;
        case 1:  glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
             break;
        default: glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
             color = 0;
             break;
    }
}



/******************************************************************************/
/*
/* GLUT glue
/*
/******************************************************************************/

static
void display(void)
{
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);

    glRotatef(fYDiff, 1,0,0);
    glRotatef(fXDiff, 0,1,0);
    glRotatef(fZDiff, 0,0,1);

    glScalef(fScale, fScale, fScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch( gleModel )
    {
        case EModelTeapot:
            glutSolidTeapot(0.6f);
            break;
        case EModelTorus:
            glutSolidTorus(0.2f, 0.6f, 64, 64);
            break;
        case EModelSphere:
            glutSolidSphere(0.6f, 64, 64);
            break;
        case EModelCube:
            drawCube();
            break;
    } // switch

    glFlush();
    glutSwapBuffers();
}


static
void play(void)
{
    int thisTime = glutGet(GLUT_ELAPSED_TIME);

    RotL += (thisTime - LastTime) * -0.001f;
    LastTime = thisTime;

    glutPostRedisplay();
}


static
void key(unsigned char keyPressed, int x, int y)
{
    switch(keyPressed)
    {
        case 'b':
            NextClearColor();
            break;
        case 'q':
        case 27:
            exit(0);
            break;
        case 't':
            if (gleModel >= EModelLast)
                gleModel = EModelFirst;
            else
                gleModel = (EModelType) ((int) gleModel + 1);
            break;
        case ' ':
            Rotate = !Rotate;

            if (!Rotate)
            {
                fXInertiaOld = fXInertia;
                fYInertiaOld = fYInertia;
            }
            else
            {
                fXInertia = fXInertiaOld;
                fYInertia = fYInertiaOld;

                // To prevent confusion, force some rotation
                if ((fXInertia == 0.0) && (fYInertia == 0.0))
                    fXInertia = -0.5;
            }
            break;
        case '+':
            fScale += SCALE_INCREMENT;
            break;
        case '-':
            fScale -= SCALE_INCREMENT;
            break;
        default:
            fprintf(stderr, "\nKeyboard commands:\n\n"
            "b - Toggle among background clear colors\n"
            "q, <esc> - Quit\n"
            "t - Toggle among models to render\n"
            "? - Help\n"
            "<home>     - reset zoom and rotation\n"
            "<space> or <click>        - stop rotation\n"
            "<+>, <-> or <ctrl + drag> - zoom model\n"
            "<arrow keys> or <drag>    - rotate model\n\n");
            break;
    }
}


static
void timer(int value)
{
    /* Increment timer */
    ftime += 0.01f;

    /* Increment wrt inertia */
    if (Rotate)
    {
        fXDiff = (fXDiff + fXInertia);
        fYDiff = (fYDiff + fYInertia);
    }

    /* Callback */
    glutTimerFunc(TIMER_FREQUENCY_MILLIS , timer, 0);
}


static
void mouse(int button, int state, int x, int y)
{
   bmModifiers = glutGetModifiers();

   if (button == GLUT_LEFT_BUTTON)
   {
      if (state == GLUT_UP)
      {
         xLast = -1;
         yLast = -1;

         if (xLastIncr > INERTIA_THRESHOLD)
            fXInertia = (xLastIncr - INERTIA_THRESHOLD)*INERTIA_FACTOR;

         if (-xLastIncr > INERTIA_THRESHOLD) 
            fXInertia = (xLastIncr + INERTIA_THRESHOLD)*INERTIA_FACTOR;

         if (yLastIncr > INERTIA_THRESHOLD) 
            fYInertia = (yLastIncr - INERTIA_THRESHOLD)*INERTIA_FACTOR;

         if (-yLastIncr > INERTIA_THRESHOLD) 
            fYInertia = (yLastIncr + INERTIA_THRESHOLD)*INERTIA_FACTOR;
      }
      else
      {
         fXInertia = 0;
         fYInertia = 0;
      }
      xLastIncr = 0;
      yLastIncr = 0;
   }
}


static
void motion(int x, int y)
{
   if ((xLast != -1) || (yLast != -1))
   {
      xLastIncr = x - xLast;
      yLastIncr = y - yLast;
      if (bmModifiers & GLUT_ACTIVE_CTRL)
      {
         if (xLast != -1)
         {
            fZDiff += xLastIncr;
            fScale += (yLastIncr)*SCALE_FACTOR;
         }
      }
      else
      {
         if (xLast != -1)
         {
            fXDiff += xLastIncr;
            fYDiff += yLastIncr;
         }
      }
   }

   xLast = x;
   yLast = y;
}


static
void reshape(int wid, int ht)
{
    float vp = 0.8f;
    float aspect = (float) wid / (float) ht;

    glViewport(0, 0, wid, ht);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, wid, ht);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
    glFrustum(-vp, vp, -vp / aspect, vp / aspect, 3, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}


static
void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_HOME:
            fXDiff    = 0;
            fYDiff    = 35;
            fZDiff    = 0;
            xLastIncr = 0;
            yLastIncr = 0;
            fXInertia = -0.5;
            fYInertia = 0;
            fScale    = 1.0;
        break;
        case GLUT_KEY_LEFT:
           fXDiff--;
        break;
        case GLUT_KEY_RIGHT:
           fXDiff++;
        break;
        case GLUT_KEY_UP:
           fYDiff--;
        break;
        case GLUT_KEY_DOWN:
           fYDiff++;
        break;
    }
}


/******************************************************************************/
/*
/* Main
/*
/******************************************************************************/

extern void CreateNoise3D();

int main( int argc, char **argv )
{
    int success = 0;
    int gl_major, gl_minor;
    GLchar *VertexShaderSource, *FragmentShaderSource;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    window = glutCreateWindow( "3Dlabs Brick Shader");

    glutIdleFunc(play);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    glutTimerFunc(TIMER_FREQUENCY_MILLIS , timer, 0);

    // Initialize the "OpenGL Extension Wrangler" library
    glewInit();

    // Make sure that OpenGL 2.0 is supported by the driver
    getGlVersion(&gl_major, &gl_minor);
    printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);

    if (gl_major < 2)
    {
        printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
        printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
        exit(1);
    }

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    NextClearColor();

    key('?', 0, 0);     // display help
#if 0
    readShaderSource("brick", &VertexShaderSource, &FragmentShaderSource);
#else
    readShaderSource("cloud", &VertexShaderSource, &FragmentShaderSource);
#endif
    success = installBrickShaders(VertexShaderSource, FragmentShaderSource);

    CreateNoise3D();

    if (success)
        glutMainLoop();
    
    return 0;
}

#include <stdio.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


static const char* DisplayErrorMessage(GLint err)
{
    switch (err) 
    {
    case GL_NO_ERROR:
	return 
	    "No error has been recorded. The value of this "
	    "symbolic constant is guaranteed to be zero.";
	break;

    case GL_INVALID_ENUM:
	return "An unacceptable value is specified for an enumerated argument.";
	break;

    case GL_INVALID_VALUE:
	return "A numeric argument is out of range.";
	    break;

    case GL_INVALID_OPERATION:
	return "The specified operation is not allowed in the current state. ";
	break;

    case GL_STACK_OVERFLOW:
	return "This command would cause a stack overflow. ";
	break;

    case GL_STACK_UNDERFLOW:
	return "This command would cause a stack underflow. ";
	break;

    case GL_OUT_OF_MEMORY:
	return
	    "There is not enough memory left to execute the command. "
	    "The state of the GL is undefined, except for the state "
	    "of the error flags, after this error is recorded.";
	break;
    }
    return NULL;
}

void checkGLError(void)
{
    GLint err = glGetError();
    if (err != GL_NO_ERROR) 
    {
	DisplayErrorMessage(err);
    }
}

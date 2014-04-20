#include "Env.hpp"
#include "Env/include.hpp"

namespace Env
{


static void glfw_error_callback(int error, const char* str)
{
    std::cerr << "Env : ERROR could not init GLFW, \"" << str << " (code " << error << ")\" " << std::endl;
}

void begin()
{
    std::cout << "Env : Env::begin() " << std::endl;

    // init GLFW
    glfwSetErrorCallback( glfw_error_callback );
    if ( !glfwInit() )
    {
        std::cerr << "Env : ERROR could not init GLFW. " << std::endl;
        throw std::runtime_error( "???" );
    }
    
    //, OpenAL, ...
}

void end()
{
    std::cout << "Env : Env::end() " << std::endl;
    
    // end GLFW
    glfwTerminate();


}

}

#include "BATB/Demo/libs/gl.hpp"
#include "BATB/Screen.hpp"
#include "BATB/Time.hpp"
#include "BATB.hpp"

////////////////////////////////////////////////////////////////////////////////
// small programs based on OpenGL Superbible 6
//


//#define PRINT_COMPILE_INFO

namespace batb
{

namespace demo
{

namespace gl
{

BATB* batb = nullptr;
GLuint make_program(const std::string& name, const GLchar* , const GLchar* , const GLchar* );
GLuint rendering_program; 
GLuint vertex_array_object;


void tests_setup(BATB* b)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    if ( !batb )
    {
        static const GLchar * vertex_shader_source = R"(#version 330 core
        layout (location = 0) in vec4 offset;
        layout (location = 1) in vec4 color;

        out VS_OUT
        {
            vec4 color;

        } vs_out;

        void main(void)
        {
            const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0), vec4(-0.25, -0.25, 0.5, 1.0), vec4( 0.25, 0.25, 0.5, 1.0));
            //const vec4 colors[3] = vec4[3]( vec4( 1.0, 0.4, 0.0, 1.0), vec4(0.3, 0.0, 0.7, 1.0), vec4( 0.1, 0.0, 0.8, 1.0) );

            vs_out.color = color;

            gl_Position = vertices[ gl_VertexID ] + offset;
        }
        )";

        static const GLchar * geometry_shader_source = R"(#version 330 core
        layout (triangles) in;
        layout (points, max_vertices = 3) out;

        void main(void)
        {
            int i;
            for (i = 0; i < gl_in.length(); i++)
            {
                gl_Position = gl_in[i].gl_Position;
                EmitVertex();
            } 
        }
        )";

        static const GLchar * fragment_shader_source = R"(#version 330 core
        in VS_OUT
        {
          vec4 color;
        } fs_in;

        out vec4 color;

        void main(void)
        {
            //color = vec4(0.0, 0.8, 1.0, 1.0);
            color = fs_in.color;
            //color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5, cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5, sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15), 1.0);
        }
        )";


        rendering_program = make_program( "OpenGLSuperBible6", vertex_shader_source, 0, fragment_shader_source ); 

        // create simple VAO
        glGenVertexArrays(1, &vertex_array_object); 
        glBindVertexArray(vertex_array_object);

        glPointSize( 40.0f );

        batb = b;
    }
}

void tests_shutdown(BATB* b)
{
    debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    glDeleteVertexArrays(1, &vertex_array_object); 
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1, &vertex_array_object);

    batb = nullptr;
}

void tests_draw()
{
    if ( batb == nullptr ) return;

debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    double currentTime = batb->time->get();

    const GLfloat color[] = { (float)std::sin(currentTime) * 0.5f + 0.5f, (float)std::cos(currentTime) * 0.5f + 0.5f,
                                      0.0f, 1.0f };
    //glClearBufferfv(GL_COLOR, 0, color);

    glDisable( GL_DEPTH_TEST );
    glDisable( GL_SCISSOR_TEST );
    glEnable( GL_BLEND );
    // Use the program object we created earlier for rendering
    glUseProgram(rendering_program);
    glBindVertexArray(vertex_array_object);
    GLfloat attrib[] = { (float)sin(currentTime) * 0.5f, (float)cos(currentTime) * 0.6f, 0.0f, 0.0f };
    // Update the value of input attribute 0
    glVertexAttrib4fv(0, attrib);

    // color
    GLfloat attribcolor[] = { 0.5f * (float)(1.0 + sin(currentTime)), 0.5f * (float)(1.0 + cos(1.7 * currentTime) ), 0.0f, 1.0f };
    glVertexAttrib4fv(1, attribcolor);

    // Draw one point
    glDrawArrays(GL_TRIANGLES, 0, 3);
}




GLuint make_shader(GLenum type, const GLchar* source)
{
#ifdef PRINT_COMPILE_INFO
    std::cout << "creating shader of type ";
    if ( type == GL_VERTEX_SHADER ) std::cout <<   "GL_VERTEX_SHADER  ";
    if ( type == GL_FRAGMENT_SHADER ) std::cout << "GL_FRAGMENT_SHADER";
    if ( type == GL_GEOMETRY_SHADER ) std::cout << "GL_GEOMETRY_SHADER";
    std::cout << ": ";
#endif

    GLuint ret = glCreateShader( type );
    glShaderSource( ret, 1, &source, NULL );
    glCompileShader( ret );

    GLint param;
    glGetShaderiv( ret, GL_COMPILE_STATUS, &param );
#ifdef PRINT_COMPILE_INFO
    if ( param == 0 ) std::cout << "ERROR: could not compile shader! ";
#endif


    // Get the log...
    GLint log_length;
    glGetShaderiv( ret, GL_INFO_LOG_LENGTH, &log_length);
    auto pstr = std::make_unique<GLchar[]>( log_length );
    
    glGetShaderInfoLog( ret, log_length, NULL, pstr.get());

#ifdef PRINT_COMPILE_INFO
    if ( log_length == 0 ) std::cout << "OK";
    else                   std::cout << std::endl << pstr.get();
    std::cout << std::endl;
#endif


    return ret;
}

GLuint make_program(const std::string& name, const GLchar* vsh_source, const GLchar* gsh_source, const GLchar* fsh_source)
{
#ifdef PRINT_COMPILE_INFO
    std::cout << "making program '" << name << "'" << std::endl;
#endif

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
#ifdef PRINT_COMPILE_INFO
    if ( param == 0 ) std::cout << "ERROR: could not link program!" << std::endl;
#endif


    // Get the log...
    GLint log_length;
    glGetProgramiv( ret, GL_INFO_LOG_LENGTH, &log_length);
    auto pstr = std::make_unique<GLchar[]>( log_length );
    glGetProgramInfoLog( ret, log_length, NULL, pstr.get() );

#ifdef PRINT_COMPILE_INFO
    if ( log_length == 0 ) std::cout << "OK";
    else                   std::cout << pstr.get();
    std::cout << std::endl;
#endif

    // Delete the shaders as the program has them now
    glDeleteShader( vsh );
    glDeleteShader( gsh );
    glDeleteShader( fsh );

    return ret; 
}

} // namespace gl
} // namespace demo
} // namespace batb


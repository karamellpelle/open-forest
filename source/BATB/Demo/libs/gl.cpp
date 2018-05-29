#include "BATB/Demo/libs/gl.hpp"
#include "BATB/Screen.hpp"
#include "BATB/Scene.hpp"
#include "BATB/Time.hpp"
#include "BATB.hpp"

#include "BATB/OGRE.hpp"
#include "OgreLogManager.h"
#include "OgreEntity.h"
#include "OgreLog.h"
#include "OgreRoot.h"
#include "OgreSceneNode.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreCamera.h"
#include "OgreResourceGroupManager.h"
#include "OgreSceneManager.h"
#include "OgreGpuProgramManager.h"
#include "OgreMeshManager.h"
//#include "RenderSystems/GL/include/OgreGLRenderSystem.h"
//#include "RenderSystems/GL3Plus/include/OgreGL3PlusRenderSystem.h"

#include "OgreRTShaderSystem.h"
#include "OgreMaterialManager.h"

////////////////////////////////////////////////////////////////////////////////
// small programs based on OpenGL Superbible 6
//


//#define PRINT_COMPILE_INFO

namespace batb
{
class BATB;

namespace demo
{

namespace gl
{


BATB* batb = nullptr;
GLuint make_program(const std::string& name, const GLchar* , const GLchar* , const GLchar* );

// OpenGL Super Bible 6 examples
void superbible6_setup();
void superbible6_shutdown();
void superbible6_draw();

// test OGRE
void ogre_setup();
void ogre_draw(const Scene& );
void ogre_shutdown();
void ogre_pause(bool );

bool is_pause = false;

void tests_setup(BATB* b)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    if ( !batb )
    {
        batb = b;

        //superbible6_setup();
        ogre_setup();
    }
}

void tests_shutdown(BATB* b)
{
    ogre_shutdown();
    //superbible6_shutdown();
}

void tests_draw(const Scene& scene)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    if ( batb == nullptr ) return;

    ogre_draw( scene );
   
    //superbible6_draw();


}

void tests_pause(bool b)
{

    ogre_pause( b );

    is_pause = b;
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





////////////////////////////////////////////////////////////////////////////////
//  OpenGL Super Bible examples

GLuint rendering_program; 
GLuint vertex_array_object;

void superbible6_setup()
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
}

void superbible6_draw()
{

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

void superbible6_shutdown()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );
    glDeleteVertexArrays(1, &vertex_array_object); 
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1, &vertex_array_object);

    batb = nullptr;
}




////////////////////////////////////////////////////////////////////////////////
//  Ogre tutorial


using namespace Ogre;

#define OGRE_VIEWPORT_ZORDER (-1)

Camera* cam = nullptr;
SceneNode* camNode = nullptr;
SceneManager* scnMgr = nullptr;

void updateCamNode(double time)
{
    double x,z;
    cossin( time, x, z );
    x *= 400;
    z *= 400;

    camNode->setPosition( x, 300, z );
    camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
}

void ogre_setup()
{
        YAML::Node yaml = YAML::LoadFile( file::static_data( "BATB/Forest/ogre.yaml" ) );

        // add resources for demo
        // TODO: find out how to use ResourceGroupManager::initialiseAllResourceGroups()
        if ( YAML::Node resources = yaml[ "resources" ] )
        {
            batb->ogre->addResourceGroupsAndInit( yaml["resources"] );
        }

    Root* root = batb->ogre->ogre_root;
    scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // -- tutorial section start --
    //! [cameracreate]
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cam = scnMgr->createCamera("myCam");
    //! [cameracreate]

    //! [cameraposition]
    //camNode->setPosition(200, 300, 400);
    //camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
    updateCamNode( batb->time->get() );
    //! [cameraposition]

    //! [cameralaststep]
    cam->setNearClipDistance(5);
    camNode->attachObject(cam);
    //! [cameralaststep]

    //scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    //! [addviewport]
    Viewport* vp = batb->ogre->ogre_renderwindow->addViewport(cam, OGRE_VIEWPORT_ZORDER ); // below other zorders
    //! [addviewport]

    //! [viewportback]
    vp->setBackgroundColour(ColourValue(0, 0, 0));
    //! [viewportback]

    //! [cameraratio]
    cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    //! [cameraratio]

    //! [ninja]
    //Entity* entity = scnMgr->createEntity("ninja.mesh");
    Entity* entity = scnMgr->createEntity("control.mesh");
    entity->setCastShadows(true);

    SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->scale( 170, 170, 170 ); 
    node->attachObject( entity );

    //! [ninja]

    //! [plane]
    Plane plane(Vector3::UNIT_Y, 0);
    //! [plane]

    //! [planedefine]
    MeshManager::getSingleton().createPlane(
            "ground",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            1500, 1500, 20, 20,
            true,
            1, 5, 5,
            Vector3::UNIT_Z);
    //! [planedefine]
    //! [planecreate]
    Entity* groundEntity = scnMgr->createEntity("ground");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    //! [planecreate]

    //! [planenoshadow]
    groundEntity->setCastShadows(false);
    //! [planenoshadow]

    //! [planesetmat]
    groundEntity->setMaterialName("Examples/Rockwall");
    //! [planesetmat]

    //! [lightingsset]
    scnMgr->setAmbientLight(ColourValue(0.0, 0.0, 0.0));
    //scnMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
    scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    //! [lightingsset]

    //! [spotlight]
    Light* spotLight = scnMgr->createLight("SpotLight");
    //! [spotlight]

    //! [spotlightcolor]
    //spotLight->setDiffuseColour(0, 0, 1.0);
    //spotLight->setSpecularColour(0, 0, 1.0);
    spotLight->setDiffuseColour(1.0, 1.0, 1.0);
    spotLight->setSpecularColour(1.0, 1.0, 1.0);
    //! [spotlightcolor]

    //! [spotlighttype]
    spotLight->setType(Light::LT_SPOTLIGHT);
    //! [spotlighttype]

    //! [spotlightposrot]
    SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    spotLightNode->attachObject(spotLight);
    spotLightNode->setDirection(OGRE_VIEWPORT_ZORDER, OGRE_VIEWPORT_ZORDER, 0);
    spotLightNode->setPosition(Vector3(200, 200, 0));
    //! [spotlightposrot]

    //! [spotlightrange]
    spotLight->setSpotlightRange(Degree(35), Degree(50));
    //! [spotlightrange]
    //
    //! [directlight]
    Light* directionalLight = scnMgr->createLight("DirectionalLight");
    directionalLight->setType(Light::LT_DIRECTIONAL);
    //! [directlight]

    //! [directlightcolor]
    directionalLight->setDiffuseColour(ColourValue(0.4, 0.4, 0.4));
    directionalLight->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
    //! [directlightcolor]

    //! [directlightdir]
    SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    directionalLightNode->attachObject(directionalLight);
    directionalLightNode->setDirection(Vector3(0, OGRE_VIEWPORT_ZORDER, 1));
    //! [directlightdir]

    //! [pointlight]
    Light* pointLight = scnMgr->createLight("PointLight");
    pointLight->setType(Light::LT_POINT);
    //! [pointlight]

    //! [pointlightcolor]
    pointLight->setDiffuseColour(0.3, 0.3, 0.3);
    pointLight->setSpecularColour(0.3, 0.3, 0.3);
    //! [pointlightcolor]

    //! [pointlightpos]
    SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    pointLightNode->attachObject(pointLight);
    pointLightNode->setPosition(Vector3(0, 150, 250));

}

void ogre_draw(const Scene& scene)
{
    if ( scnMgr )
    {
        batb->ogre->sceneBegin( scene );

        uint w,h;
        batb->screen->getSize( w, h );
        //
        cam->setAspectRatio( (float_t)(w) / (float_t)( h) );

        updateCamNode( batb->time->get() );
        batb->ogre->outputCamera( cam );

        batb->ogre->sceneEnd();
    }
}

void ogre_pause(bool pause)
{
    if ( pause && !is_pause )
    {
        // create a new viewport
        batb->ogre->ogre_renderwindow->removeViewport( OGRE_VIEWPORT_ZORDER );
    }
    if ( !pause && is_pause )
    {
        // add a new viewport. the previous was removed
        Viewport* vp = batb->ogre->ogre_renderwindow->addViewport(cam, OGRE_VIEWPORT_ZORDER ); // below other zorders
        vp->setBackgroundColour(ColourValue(0, 0, 0));
    }
}

void ogre_shutdown()
{
    batb->ogre->ogre_renderwindow->removeViewport( OGRE_VIEWPORT_ZORDER );

    // this releases (?) created objects
    batb->ogre->ogre_root->destroySceneManager( scnMgr );

    camNode = nullptr;
    cam = nullptr;
    scnMgr = nullptr;
}



} // namespace gl
} // namespace demo
} // namespace batb


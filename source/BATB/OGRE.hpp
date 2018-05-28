//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#ifndef BATB_OGRE_OGRE_HPP
#define BATB_OGRE_OGRE_HPP
#include "BATB/ModuleBATB.hpp"


// forward declare Ogre classes
namespace Ogre
{
    class LogManager;
    class Log;
    class Root;
    class RenderWindow;
    class RenderTarget;
    class Camera;
    class Viewport;
    class RenderSystem;

    namespace RTShader
    {
        class ShaderGenerator;
    }
}


namespace batb
{
class Scene;


namespace ogre
{

class SGTechniqueResolverListener;



class OGRE : public ModuleBATB
{
public:
    OGRE(BATB* b) : ModuleBATB( b ) { }

    // setup
    void begin(const std::string& );
    void end();

    ////////////////////////////////////////////////////////////////////////////////
    // Scene rendering
    //

    // should Ogre not be ignored each frame?
    void enabled(bool e);

    // start/end frame (each iteration, i.e. inside IterationRun::iterate)
    void frameBegin();
    void frameEnd();

    // set Scene for rendering 
    // only Ogre output (i.e. outputCamera/outputViewport) should be done 
    // inside, since other gl may mess up
    void sceneBegin(const Scene& );
    void sceneEnd();

    // render into Scene, using camera
    void outputCamera(Ogre::Camera* );
    // render into Scene, using a viewports camera
    void outputViewport(Ogre::Viewport* );
    ////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////
    // for each frame, let Ogre render everything inside it
    // (no frameXXX, sceneXXX, outputXXX should be used)
    void output(const Scene& );
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // 
    void addResourceGroup(const std::string& group_name, const YAML::Node& );
    void addResourceGroupsAndInit(const YAML::Node& );

      
    Ogre::LogManager*                ogre_logmanager = nullptr;
    Ogre::Root*                      ogre_root = nullptr;
    Ogre::RenderSystem*              ogre_rendersystem = nullptr;
    Ogre::RenderWindow*              ogre_renderwindow = nullptr; // this is a Ogre::RenderTarget
    Ogre::RTShader::ShaderGenerator* ogre_shader_generator = nullptr;
    SGTechniqueResolverListener*     ogre_technique_resolver_listener = nullptr;
    // ^ see Ogre::ApplicationContext::setup()/initialiseRTShaderSystem(), Ogre::SGTechniqueResolverListener of OgreBites
    //   and https://ogrecave.github.io/ogre/api/1.11/rtss.html

private:
    void initPrograms();
    void initRTSS();

    // Name of current ogre_rendersystem
    std::string ogre_rendersystem_name_ = "OpenGL 3+ Rendering Subsystem";

    bool enabled_ = false;
    bool ogre_statistics_ = true;


};


} // namespace ogre

} // namespace batb

#endif


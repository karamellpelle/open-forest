 macOS Sierra

* glew submodule does not build; relies on installed (brew)
* create open-forest-ogre mirror
* install more dependencies for ogre:
  - zziplib
  - freeimage 
  - +++; see Ogre CMake configuration


# TODO
* Notify and Console from run::Run into BATB ??
* language files
* `reset()` for objects in forest::World
* Indefinite RunWork
* load forest world by RunWork
* TBEditField. up key can be read?
* remove template LoadWorker
* IterationRunWork and TB

* rename `float_t` ?  `floating_t` ?
* define precision of glm (vec, mat, etc) the same as float_t 
* OpenGL 3+, premultiplied colors by default
* ThreadGLContext: exception on 'current'
* class Key { shared_ptr<KeyImpl> impl_; }
* implement Log: current indentation, error/warning tags, etc
* implement batb/gui/tb_system.cpp, in a platform independent way.
* stb_image as individual module (currently using the one from nanovg..). used by nanovg, turbobadger
* FPS and other BATB-stats
* define invariants (OpenGL, ...)
* find a suitable UDP-library (http://www.codeofhonor.com/blog/choosing-a-game-network-lib)
* GLEW as git submodule 
* glext.h (part of official OpenGL Registry) as git submodule, or something

## CMake

* control debug build in CMakeLists.txt
* let CMake add compile options, don't rely on custom compiler like gcc
* check for required packages: glew, boost, openal, etc.
* debug build. flags
* no rtti for turbobadger
* remove cmake/libs/alure

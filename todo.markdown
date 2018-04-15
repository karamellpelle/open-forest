filepath and Module config
See OSXCocoaCOntext::clone() for how threading is done!


# TODO
* world include all parts
* 1 lib: game
* 1 lib: file
* create global variable 'batb'!
* World::load -> World::begin?
* fix nanovg+ogre debug errors! `NVG_DEBUG`
* fix segfault (from Ogre) during shutdown on seperate thread
* uppercase Run/Event.hpp, etc
* begin/end iterations
* forestworld: loading
* use metric system for terrain and all models!!!
* Notify and Console from run::Run into BATB ??
* language files
* `reset()` for objects in forest::World
* load forest world by RunWork
* remove template LoadWorker
* remove _emptylist_

* define precision of glm (vec, mat, etc) the same as float_t 
* OpenGL 3+, premultiplied colors by default
* `class Key { shared_ptr<KeyImpl> impl_; }`
* extend functionality of Log: error/warning levels with indentation
* stb_image as individual module (currently using the one from nanovg..). used by nanovg, turbobadger
* define invariants (OpenGL, ...)
* find a suitable UDP-library (http://www.codeofhonor.com/blog/choosing-a-game-network-lib)

## CMake

* control debug build in CMakeLists.txt
* no rtti for turbobadger

## macOS
* GLSL shader for terrain fails: undeclared identifier: worldPos!

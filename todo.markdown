See OSXCocoaCOntext::clone() for how threading is done!

# TODO
* Notify and Console from run::Run into BATB ??
* language files
* `reset()` for objects in forest::World
* Indefinite RunWork
* load forest world by RunWork
* TBEditField. up key can be read?
* remove template LoadWorker
* IterationRunWork and TB

* rename `float_t` ?  `floating_t` ? because `float_t` is found in other c libs
* define precision of glm (vec, mat, etc) the same as float_t 
* OpenGL 3+, premultiplied colors by default
* ThreadGLContext: exception on 'current'
* `class Key { shared_ptr<KeyImpl> impl_; }`
* extend functionality of Log: error/warning levels with indentation
* stb_image as individual module (currently using the one from nanovg..). used by nanovg, turbobadger
* FPS and other BATB-stats
* define invariants (OpenGL, ...)
* find a suitable UDP-library (http://www.codeofhonor.com/blog/choosing-a-game-network-lib)

## CMake

* control debug build in CMakeLists.txt
* no rtti for turbobadger


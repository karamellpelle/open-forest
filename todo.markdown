# fruxo
nullptr no check for during read


# TODO
* noitify fade out
* do not hardcode .tb.txt file paths. find out how to read our custom widgets.
  we should define and use OnInflate() actively.
* remove demo::background
* TB: register other font renderers (tb_config.h needed too)
* KeyButton -> KeyKeyboardButton. `Keys::window_ -> Keys::glfw_window_`
* callback for windows: close app
* container Key's shall not update/step children, because 'Keys' does it for all Key's!!
* create global variable 'batb'?
* World::load -> World::begin?
* fix nanovg+ogre debug errors! `NVG_DEBUG`
* fix segfault (from Ogre) during shutdown on seperate thread. see OSXCocoaCOntext::clone() for how threading is done!
* begin/end iterations
* `using float_t = double_t;` in include
* forestworld: loading
* use metric system for terrain and all models!!!
* language files
* `reset()` for objects in forest::World
* load forest world by RunWork
* remove template LoadWorker
* remove _emptylist_

* define precision of glm (vec, mat, etc) the same as float_t 
* OpenGL 3+, premultiplied colors by default
* stb_image as individual module (currently using the one from nanovg..). used by nanovg, turbobadger
* define invariants (OpenGL, ...)
* find a suitable UDP-library (http://www.codeofhonor.com/blog/choosing-a-game-network-lib)

## CMake

* control debug build in CMakeLists.txt
* no rtti for turbobadger

## macOS
* GLSL shader for terrain fails: undeclared identifier: worldPos!

# credits
* `<div>Icons made by <a href="https://www.flaticon.com/authors/smashicons" title="Smashicons">Smashicons</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>`
* `<div>Icons made by <a href="http://www.freepik.com" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>`

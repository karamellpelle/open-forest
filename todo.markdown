# fruxo
nullptr no check for during read


# TODO

* tb skin for Key::nameGUI()
* worlds refer to each others with pointers
* make demo::World a subworld of run::World instead of a custom world
  for IterationRunDemo
* TBMain setup
* Toggle fullscreen Key
* IterationRunDemo into BATB::Run
* load Keys from YAML
* callback for windows: close app
* mouse and map view
* create global variable 'batb'?
* begin/end iterations
* `using float_t = double_t;` in include
  - define precision of glm (vec, mat, etc) the same as float_t 
* remove _emptylist_

* OpenGL 3+, premultiplied colors by default
* stb_image as individual module (currently using the one from nanovg..). used by nanovg, turbobadger
* define invariants (OpenGL, ...)
* YAML configuration
  - static files vs. dynamic files: settings override. for example keyboard settings, how should this be done?
    can YAML::Node tree easily replace elements by providing new nodes to the tree?
  - saving configuration (dynamic files)

## threading
* fix nanovg+ogre debug errors! `NVG_DEBUG`
* fix segfault (from Ogre) during shutdown on seperate thread. see OSXCocoaCOntext::clone() for how threading is done!
* fix IterationRunWork to load run::World, forest::World, etc on seperate thread. 
* remove template LoadWorker

## Ogre3d
* load/unload terrains etc, making possible to start over
* fix terrain rendering (it's black on macOS)

## game
* wrap Iteratations into some class with shared_ptr to easily work with Iteration's and have control over their lifetimes

## forest
* `reset()` for objects in forest::World
* use metric system for terrain and all models!!!
* World::load -> World::begin. do something better than WorldLoader? it has to work with iterations loading on different threads
* make AL sources for control punches play in 3D

## demo
* no al in demo file.
  - find out what to do with the easter egg. can Alure delete buffer on finish?
* mouse and show map:
  - right
* accelerated movement with arrow buttons
* restart Demo (ogre) from main menu

## Keys
* read key from YAML
* container Key's shall not update/step children, because 'Keys' does it for all Key's!!
* KeyButton with modifiers
* KeyButton -> KeyKeyboardButton

## tb
* language files
* notify 
  - fade out widgets
  - handle keypress for infinity duration
  - play alert sound?
* do not hardcode .tb.txt file paths. find out how to read our custom widgets.
  we should define and use OnInflate() actively.
* create custom skin: fonts, windows, etc
* better logo
* TB: register other font renderers (tb_config.h needed too)

## CMake

* control debug build in CMakeLists.txt
* no rtti for turbobadger

## macOS
* GLSL shader for terrain fails: undeclared identifier: worldPos!

# credits
* `<div>Icons made by <a href="https://www.flaticon.com/authors/smashicons" title="Smashicons">Smashicons</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>`
* `<div>Icons made by <a href="http://www.freepik.com" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>`

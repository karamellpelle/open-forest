# fruxo
nullptr no check for during read


# TODO

* padding for TBNotify
* getKey -> getKeyboardButton in Keys
* use Keys::create<KeyT>( cons ) in Keys.cpp
* Value reads values from Value.yaml file. 1 big file, or 1 per module?
* TBTickerLine statusline in top of console :)
* tb skin for Key::nameGUI()
* tutorial for controls in forest.
* worlds refer to each others with pointers
* make demo::World a subworld of run::World instead of a custom world
  of IterationRunDemo
* TBMain setup
* IterationRunDemo into BATB::Run
* callback for windows: close app
* mouse and map view
* create global variable 'batb'?
* begin/end iterations
* `using float_t = double_t;` in include
  - define precision of glm (vec, mat, etc) the same as float_t 
* remove _emptylist_
* better memory handling! use `shared_ptr`'s more.

* OpenGL 3+, premultiplied colors by default
* stb_image as individual module (currently using the one from nanovg..). used by nanovg, turbobadger
* define invariants (OpenGL, ...)
* YAML configuration
  - static files vs. dynamic files: settings override. for example keyboard settings, how should this be done?
    can YAML::Node tree easily replace elements by providing new nodes to the tree?
  - saving configuration (dynamic files)

## Log
* handle flush. important for debugging
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

## Forest
* `reset()` for objects in forest::World
* use metric system for terrain and all models!!!
* World::load. do something better than WorldLoader? it has to work with iterations loading on different threads
* let AL sources for control punches play in 3D, not 2D as it seems to be now

### controls
* add more settings in the "KeySet" YAML::Node than the ones used to create the Key's, for example camera movement speed and map scroll speed
* accelerated movement with arrow buttons
* while showing map, make it possible to use mouse scroll wheel (?) to
  control the opacity of map. hence it is possible to read map while running,
  and also don't switch screen too much thus keeping orientation. this value
  should also be possible to set by Console
* forest and map
  - right click: show map
  - right click + scroll wheel: rotate map
  - right click + shift: rotate map
  - left click + right click: move map
  - left click + right click + shift: zoom map
  - XXX: orientate map (TODO: this is a bit cheating from the orienteering skills. hence
    make a setting to enable/disable this possibitlity)
  - YYY: postition in map and orientate map. this is absolutely cheating :)

## demo
* no al in demo file.
  - find out what to do with the easter egg. can Alure delete buffer on finish?

## Keys
* configure more details from YAML::Node, like KeyMouseScroll speed and KeyAlpha values.
* KeyCombine: use children for alpha(), typically for things like Ctrl+Shift+X 
* memory handling (create / remove )
* Key constructor: `BATB*`

## tb
* language files
* notify 
  - fade out widgets
  - handle keypress for infinity duration
  - play alert sound?
* setup custom TBWidgets with no hardcoded paths
* do not hardcode .tb.txt file paths. find out how to read our custom widgets.
  we should define and use OnInflate() actively.
* create custom skin: fonts, windows, etc
* better logo
* TB: register other font renderers (tb_config.h needed too)

## other game types
* get lost: the runner is placed randomly in the forest and have to find out where in the map.

## CMake

* control debug build in CMakeLists.txt
* no rtti for turbobadger

## macOS
* GLSL shader for terrain fails: undeclared identifier: worldPos!

# credits
* `<div>Icons made by <a href="https://www.flaticon.com/authors/smashicons" title="Smashicons">Smashicons</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>`
* `<div>Icons made by <a href="http://www.freepik.com" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>`

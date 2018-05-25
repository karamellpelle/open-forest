
# TODO
* remove demo/libs/ogreterrain, and clean up data directories
* forest::World: set Weater (skybox, light, etc.) after terrain loaded.
  move the light creation from terrain::load() into the weather setup!
* attach Camera to SceneNode and use that. new in v1.10
* remove OgreTerrain demo, stay with forest
* fix OGRE data: static vs dynamic. 
* helpers/tb.hpp -> tb/helpers.hpp
* custom TB-widgets have empty constructors since all TB-widgets should be possible.
  to read from .tb.txt file's (TBWidgetFactory).
  - TBMain
  - TBConsole?
* no indent->pop() for exceptions; done by destructors regarding exception or not
* clipboard support TB (tb_system.h)
* use the implemetation of Ogre::renderOneFrame(Real timeSinceLastFrame)?
* nanovg debug if defined in yaml
* clean up events during shutdown
* better reading custom TB widgets. read custom props for for example console
* let cmd's use boost program_options
* Notify and message Key
  - remove Key from NotifyMessage and put into TBNotify. this key should call message->finish() on
    first NotifyMessage. 
  - implement TBNotifyStep and message lifetimes
* move event (BeginEvents class) from Demo to demo::World. also Forest
* rewrite Event. use C++14. handle memory better.
* find better, consequent names. for example the name 'begin' is used for setup a permanent object (ex: `BATB::begin( filepath )`) but also set up a frame (ex: `begin( run.scene )`
* make demo::World a subworld of run::World. then remove 'IterationRunDemo::demoWorld()' from 
  IterationRunDemo because Iteration's are never bound to specific worlds; they work on every
  world. in our case we only have 1 run::World so it works, but in the wrong way.
* padding for TBNotify
* Value reads values from Value.yaml file. 1 big file, or 1 per module?
* TBTickerLine statusline in top of console :)
* tb skin for Key::nameGUI()
* tutorial for controls in forest.
* worlds refer to each others with pointers: prevents recompilation and makes it easier to set
  up a subworld after the world which own is has been properly set up. make a rule: subworld 
  uses pointers (i.e. unique_ptr) and other are objects, lik Event's and Scene.
* TBMain setup
* callback for windows: close app
* mouse and map view
* create global variable 'batb'?
* begin/end iterations
* `using float_t = double_t;` in include
  - define precision of glm (vec, mat, etc) the same as float_t 
* remove _emptylist_ - the bugfix for a compiler I had to use in a build.
* better memory handling! use `shared_ptr`'s more.

* OpenGL 3+, premultiplied colors by default
* stb_image as individual CMake library (we are currently using the one from nanovg..) but Turbobadger (and maybe
  open-forest too) needs stb_image.
* define invariants (OpenGL, ...)
* YAML configuration
  - static files vs. dynamic files: settings override. for example keyboard settings, how should this be done?
    can YAML::Node tree easily replace elements by providing new nodes to the tree?
  - saving configuration (dynamic files)

## Log
## Event
* conseqent case naming: i.e. camelCase
## Run
* notify 
  - fade out widgets
  - handle keypress for infinity duration
  - play alert sound?

## threading
* fix segfault (from Ogre) during shutdown on seperate thread. see OSXCocoaCOntext::clone() for how threading is done!
* fix IterationRunWork to load run::World, forest::World, etc on seperate thread. 
* remove template LoadWorker

## Ogre3d
* fix nanovg+ogre debug errors! (errors every frame when nanovg has been initialized with the `NVG_DEBUG` flag)
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

# dev
events may come from the outside (i.e. BATB and its modules (for example Screen close) or inside (for example runner punches control). In any case the events are stored in the corresponding world. Therefore events from the outside has to be transfered to the world. 
## naming convention
* classes (which includes enum's): `MyClass`
* methods: `myMethod`
* functions (not method's or static function in classes): `my_function`
* variables (including public and protected of classes): `my_variable`
* private member variables (not methods): `my_variable_`
* enum elements: `EnumItem`

## GUI
* customs widgets should be implemented with default, empty constructor in order to make it read from a .tb.txt file.
  other arguments to the widget should given afterwards

## subworlds / events output and step
iterate world = do
   output subworld
   output world
   update events (world + subworld)
   step world
   step subworld
   take events
   think

iterate world = do
    update events
    step world
    take events : XXX -> xxx::World 
    iterate stack subworld (output subworld, step subworld, think subworld)
    output world

    think

iterate world = do
    update events
    output subworld
    output world
    
    step subworld
    take events 

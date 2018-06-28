
# TODO


* find suitable values for Control alure::Source
* let forest::World contain pointers to objects (Runners, Controls, etc.).
* fix Camera: set Ogre::Camera's SceneNode from BATB::Forest::Camera's Aim matrix.
  something wrong happesn if 'setFixedYawAxis( true );' is not called
  also fix controls and Z direction. An Ogre::Camera uses -Z for view direction.
* setup GUI from .yaml
* class Aim : public glm::mat4
* review the GL::xxxBegin()/End() functions in GL.cpp. remove as much as possible.
* OGRE/AL enabled() necessary ??
* fix OGRE data: static vs dynamic. 
* custom TB-widgets have empty constructors since all TB-widgets should be possible.
  to read from .tb.txt file's (TBWidgetFactory).
  - TBMain
  - TBConsole?
* clipboard support TB (tb_system.h)
* clean up events during shutdown
* let cmd's use boost program_options
* rewrite Event. use C++14. handle memory better.
* make demo::World a subworld of run::World. then remove 'IterationRunDemo::demoWorld()' from 
  IterationRunDemo because Iteration's are never bound to specific worlds; they work on every
  world. in our case we only have 1 run::World so it works, but in the wrong way.
* Value reads values from Value.yaml file. 1 big file, or 1 per module?
* TBTickerLine statusline in top of console :)
* tb skin images for non-keyboardbutton keys Key::nameGUI()
* tutorial for controls in forest.
* mouse and map view
* create global variable 'batb'? Answer: NO! BATB is the toolbox to work **on** World's, hence make it impossible to use it inside World's
* `using float_t = double_t;` in include?
  - define precision of glm (vec, mat, etc) the same as float_t 
* remove _emptylist_ - the bugfix for a compiler I had to use in a build.
* better memory handling! use `shared_ptr`'s more.

* OpenGL 3+, premultiplied colors by default
* stb_image as individual CMake library (we are currently using the one from nanovg..) but Turbobadger (and maybe
  open-forest too) needs stb_image.
* define invariants (OpenGL, ...)
* YAML configuration
  - user configuration overrides values, because user configuration can have comments
  - static files vs. dynamic files: settings override. for example keyboard settings, how should this be done?
    can YAML::Node tree easily replace elements by providing new nodes to the tree?
  - saving configuration (dynamic files)

## Log
## Event
* conseqent case naming: i.e. camelCase
## Run
* notify 
  - play alert sound?

## threading
* fix IterationRunWork to load run::World, forest::World, etc on seperate thread. 

## Ogre3d
* fix nanovg+ogre debug errors! (errors every frame when nanovg has been initialized with the `NVG_DEBUG` flag)
* load/unload terrains etc, making possible to start over
* use Ogre v2.x when macOS support becomes better/stable

## game
* wrap Iteratations into some class with shared_ptr to easily work with Iteration's and have control over their lifetimes

## Forest
* `reset()` for objects in forest::World
* use metric system for terrain and all models!!!

### controls
* add more settings in the "KeySet" YAML::Node than the ones used to create the Key's, for example camera movement speed and map scroll speed
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

## Keys
* configure more details from YAML::Node, like KeyMouseScroll speed and KeyAlpha values.
* KeyCombine: use children for alpha(), typically for things like Ctrl+Shift+X 
* memory handling (create / remove )

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

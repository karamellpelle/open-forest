Open Forest
--------------------

This was originally a fork of an orienteering game I found on GitHub, ["Beat About The Bush]( 
https://github.com/jarvinet/orienteering-game), but it is now completely rewritten.
The old game is still runnable, but it needs additional libs in order to build (plib, ...).

The intention is to build up a foundation for an open source orienteering game. Despite the sport of 
orienteering is small, there are multiple commercial orienteering games available (Catching Features, 
Suunnistussimulaattori, Oriantica), and all these costs money. I don't see the point of having so many
commercial games, for something that probably not will generate enough money for an high-end product.
Also, a lot of orienteers are good programmers/developers. We'll see how this goes...

This project is currently under construction, there needs more work (from me) before I expect others
to join. I try to work on this project when I have time.


Programming
--------------------

We shall aim at using modern programming and libraries. This means OpenGL 3.3+ (shaders, buffers, 
vertex arrays, etc.), C++11, etc.

#### Libraries

Below are suggestions of open source libraries to use. Please add (better) alternatives.

graphics          : [**OpenGL**](https://www.opengl.org/wiki/), [**GLEW**](http://glew.sourceforge.net), [**GLFW**](http://www.glfw.org)  
sound             : [**OpenAL Soft**](https://github.com/kcat/openal-soft), [**ALURE**](http://kcat.strangesoft.net/alure.html), [MikMod ?](http://mikmod.sourceforge.net)  
input             : [**GLFW**](http://www.glfw.org)  
GUI               : [**turbobadger**](https://github.com/fruxo/turbobadger)  
config            : [**YAML**](http://yaml.org), [tinyxml2](http://www.grinninglizard.com/tinyxml2)  
UDP network       : [libuv ?](https://github.com/libuv/libuv), 
                    (boost ASIO ?)  
3D rendering      : [**OGRE**](http://www.ogre3d.org)  
2D rendering      : [nanovg](https://github.com/memononen/nanovg)  
font rendering    : (using turbobadger or nanovg)  
packaging         : [link 0](http://gamedev.stackexchange.com/questions/37648/how-can-you-put-all-images-from-a-game-to-1-file), 
                    [link 1](http://en.wikipedia.org/wiki/List_of_archive_formats)  
collision         : do we need collision?,
                    [Bullet3D ?](https://github.com/bulletphysics/bullet3)  
linear            : [GLM](http://glm.g-truc.net)  
3D animation      : [**OGRE**](http://www.ogre3d.org)  
video             : [OGRE ?](http://www.ogre3d.org), 
                    [libTheora ?](http://www.theora.org/)  
c++               : (boost.filesystem ?),
                    (boost.hash ?),
                    (boost.integer ?),
                    (boost.geometry?),
                    (boost.predef ?),
                    (boost.random ?),
                    (boost.string_algo ?),
                    (boost.system_library ?),
                    (boost.program_options ?),
                    (boost.asio ?),
                    (boost.math ?)

Currently, `GLFW`, `Ogre`, `nanovg`, `turbobadger`, `YAML`, `tinyxml2`, `GLM`  are up and running. 


Building
--------------------

The current libraries are needed:

- boost (I have version 1.55.0)
- GLEW  (I have version 1.10.0)

In addition, this git project depends on submodules. Run

    $ ./build.sh init-submodules

the first time to get the needed submodules. 

Building can then be done with

    $ ./build.sh

which creates a program `build/open-forest`.


Contact
----------------
<carljsv@student.matnat.uio.no>

<karamellpelle@hotmail.com>

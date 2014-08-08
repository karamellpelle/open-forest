Open Forest
--------------------

This is a fork of an orienteering game I found on GitHub, https://github.com/jarvinet/orienteering-game.

The intention is to build up a foundation for an open source orienteering game. Despite the sport of 
orienteering is small, there are multiple commercial orienteering games available (Catching Features, 
Suunnistussimulaattori, Oriantica), and all these costs money. I don't see the point of having so many
commercial games, for something that probably not will generate enough money for an high-end product.
Also, a lot of orienteers are good programmers/developers. We'll se how this goes...

This project is currently under construction, there needs more work (from me) before I expect others
to join. I try to work on this project when I have time...


Programming
--------------------

We shall aim at using modern programming and libraries . This means OpenGL 3.3+ (shaders, buffers, 
vertex arrays, etc.), C++11, etc.

Currently, these libraries are intented to be used. Please fill in (better) alternatives...

    graphics          : OpenGL, GLFW, GLEW
    sound             : (OpenAL ?)
    input             : GLFW
    GUI               : https://github.com/fruxo/turbobadger          
                        (CEGUI ?)
                        (JUCE ?)
                        (https://github.com/betajaen/gorilla ?)
    config            : XML, https://github.com/leethomason/tinyxml2
                        (I don't like XML. YAML looks like a better candidate)
    UDP network       : (boost ASIO ?)
                        (https://github.com/joyent/libuv/ ?)
    3D rendering      : OGRE                                          
                        (OpenSceneGraph ?)
                        (OpenSG ?)
                        (Horde3D ?)
                        (SpeedTree :))
    2D rendering      : https://github.com/memononen/nanovg
    font rendering    : (using turbobadger/nanovg) 
    collision         : (do we need this?)
                        (Bullet3D ?)
    linear            : GLM
    3D animation      : (OGRE ?)
    video             : OGRE
                        (libTheora?)
    c++               : (boost.filesystem ?)
                        (boost.hash ?)
                        (boost.integer ?)
                        (boost.geometry?)
                        (boost.predef ?)
                        (boost.random ?)
                        (boost.string_algo ?)
                        (boost.system_library ?)
                        (boost.program_options ?)
                        (boost.asio ?)
                        (boost.math ?)


Currently, GLFW, tinyxml2, nanovg, GLM, turbobadger, are up and running. 


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

, creating a program `build/open-forest`.



old-BATB
---------------------
The old game "Beat About The Bush" is still runnable in this fork, and
it needs additional libs (plib, ..., see the original repository)


Contact
----------------
<carljsv@student.matnat.uio.no>

<karamellpelle@hotmail.com>

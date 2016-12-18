![](https://raw.githubusercontent.com/karamellpelle/open-forest/master/data/static/batb/openforest-512x256.png "OpenForest")

Open Forest
--------------------

This was originally a fork of an orienteering game I found on GitHub, ["Beat About The Bush]( 
https://github.com/jarvinet/orienteering-game), but it is now completely rewritten.
The old game is still runnable (?), but it needs additional libs in order to build (plib, ...).

My intention is to build up a foundation for an open source orienteering game. Despite the sport of 
orienteering is small, there are multiple commercial orienteering games available (Catching Features, 
Suunnistussimulaattori, Oriantica), and all these costs money. I don't see the point of having so many
commercial games for something that probably not will generate enough money for a high-end product.
There are many good programmers/developers in the orienteering community!

This project is currently under construction, there needs more work (from me) before I expect others
to join. I try to work on this project when I have time.


Programming
--------------------

We shall aim at using modern programming and libraries (OpenGL 3.3+, C++11/14/17, etc.)

#### Libraries

Below are suggestions of open source libraries to use. Please add (better) alternatives.

graphics          : [**OpenGL**](https://www.opengl.org/wiki/), [**GLEW**](http://glew.sourceforge.net), [**GLFW**](http://www.glfw.org)  
sound             : [**ALURE**](http://kcat.strangesoft.net/alure.html)  
input             : [**GLFW**](http://www.glfw.org)  
GUI               : [**turbobadger**](https://github.com/fruxo/turbobadger)  
config            : [**YAML**](http://yaml.org), [tinyxml2](http://www.grinninglizard.com/tinyxml2)  
UDP network       : ?  
3D rendering      : [**OGRE**](http://www.ogre3d.org)  
2D rendering      : [**nanovg**](https://github.com/memononen/nanovg)  
font rendering    : (using turbobadger or nanovg)  
packaging         : [gamedev.stackexhange.com](http://gamedev.stackexchange.com/questions/37648/how-can-you-put-all-images-from-a-game-to-1-file), 
                    [wikipedia.org](http://en.wikipedia.org/wiki/List_of_archive_formats)  
collision         : do we need collision?,
                    [Bullet3D ?](https://github.com/bulletphysics/bullet3)  
linear            : [**GLM**](http://glm.g-truc.net)  
3D animation      : [**OGRE**](http://www.ogre3d.org)  
video             : [OGRE ?](http://www.ogre3d.org), 
                    [libTheora ?](http://www.theora.org/)  
c++               : [**boost**](http://www.boost.org/)
                    (filesystem, hash, integer, geometry, predef, system_library, program_options, asio, math ??)

Currently, `GLFW`, `Ogre`, `nanovg`, `turbobadger`, `ALURE`, `YAML`, `tinyxml2`, `GLM`  are up and running. 


Building
--------------------

_open-forest_ depends on:

  * glew
  * boost
  * mpg123
  * libzzip
  * freeimage
  * freetype
  * OpenAL

It also depends on git submodules. Run
    
    $ git submodule init
    $ git submodule update

the first time pull in these.

Building can then be done with

    $ ./build.sh

which creates a program you can run with `./open-forest.sh`.

## macOS

I am working on build on macOS Sierra. Use [homebrew](www.brew.sh) to install what you need:

    $ brew install cmake
    $ brew install glew
    $ brew install boost
    $ brew install mpg123
    $ brew install libzzip
    $ brew install freeimage
    $ brew install freetype


Screenshots
----------------

Some [screenshots](data/meta/screenshots/screenshots.markdown) showing the terrain rendering (based on Ogre3D sample).

Contact
----------------
<carljsv@student.matnat.uio.no>

<karamellpelle@hotmail.com>

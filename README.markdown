![](https://raw.githubusercontent.com/karamellpelle/open-forest/master/data/static/batb/openforest-512x256.png "OpenForest")

Open Forest
--------------------

This was originally a fork of an orienteering game I found on GitHub, ["Beat About The Bush]( 
https://github.com/jarvinet/orienteering-game), but it is now completely rewritten.
The old game is still runnable through Open Forest, but then the project will have more dependencies in order to build (i.e. `plib`).

My intention has been to build up a foundation for an open source orienteering game. Despite the sport of orienteering is small, there are multiple commercial orienteering games available (Catching Features, Suunnistussimulaattori, Oriantica). I don't see the point of having so many commercial games for something that probably will not generate enough money for a high-end product. And there are many good programmers/developers in the orienteering community.

This project builds (on macOS, as of April 2018) and the libraries are up and running, so collaborative development is absolutely possible although I ideally should have finished more work first (I'm in lack of time). The program's structure is based on how I've been writing functional games in Haskell. The thing is to abandon the traditional "main loop" and consider a game as a _World_ (with subworlds) that is iterated by _Iteration_'s. See [computergames.pdf](https://github.com/karamellpelle/grid/blob/master/computergames.pdf). This makes the program flow a lot easier to implement and opens up possibilites that are complicated to do with a traditional main loop. It can be considered as a MVC pattern where _World_ is the model, and _Iteration_'s are control and view.

The [Ogre3D](https://www.ogre3d.org/) graphics library is used for 3D rendering. It has very good forest rendering capabilities, for example see this [Ogre3D based](https://forums.ogre3d.org/viewtopic.php?f=11&t=35922) game: [clip A](https://youtu.be/4QIHzn1PrxY?t=12m20s), [clip B](https://youtu.be/PWSg4olxaYE?t=5m9s), [clip C](https://youtu.be/CXr82Gtbk1Y?t=1m57s).


Programming
--------------------

We shall aim at modern programming (OpenGL 3.3 Core, C++14, modern libraries etc.)

#### Libraries

Below are suggestions of open source libraries to use. Please add (better) alternatives.

  * graphics          : [**OpenGL**](https://www.opengl.org/wiki/), [**GLEW**](http://glew.sourceforge.net), [**GLFW**](http://www.glfw.org) (or [glad](https://github.com/Dav1dde/glad)?, see [Loading Library](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library))  
  * sound             : [**ALURE**](http://kcat.strangesoft.net/alure.html)  
  * input             : [**GLFW**](http://www.glfw.org)  
  * GUI               : [**turbobadger**](https://github.com/fruxo/turbobadger)  
  * config            : [**YAML**](http://yaml.org), [tinyxml2](http://www.grinninglizard.com/tinyxml2)  
  * UDP network       : [???](http://www.codeofhonor.com/blog/choosing-a-game-network-lib)  
  * 3D rendering      : [**OGRE**](http://www.ogre3d.org)  
  * 2D rendering      : [**nanovg**](https://github.com/memononen/nanovg)  
  * font rendering    : (using turbobadger or nanovg)  
  * packaging         : [gamedev.stackexhange.com](http://gamedev.stackexchange.com/questions/37648/how-can-you-put-all-images-from-a-game-to-1-file), [wikipedia.org](http://en.wikipedia.org/wiki/List_of_archive_formats)  
  * collision         : do we need collision?, [Bullet3D ?](https://github.com/bulletphysics/bullet3)  
  * linear            : [**GLM**](http://glm.g-truc.net)  
  * 3D animation      : [**OGRE**](http://www.ogre3d.org)  
  * video             : [OGRE ?](http://www.ogre3d.org), [libTheora ?](http://www.theora.org/)  
  * c++               : [**boost**](http://www.boost.org/) (filesystem, hash, integer, geometry, predef, system_library, program_options, asio, math ??)  


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

I been having some problems with [Ogre3D](https://github.com/karamellpelle/open-forest-ogre) on macOS, the problem seems to lie in the files `Components/Terrain/src/OgreTerrainMaterialGeneratorA.cpp` and `Components/Terrain/src/OgreTerrainMaterialShaderHelperGLSL.cpp`. But maybe it works better on newer Apple computers; mine is a MacBook Pro 13\" Late 2011. And maybe the new 2.1 version of Ogre3D works better.

Screenshots
----------------

Some [screenshots](data/meta/screenshots/screenshots.markdown) showing the terrain rendering.

Contact
----------------

<karamellpelle@hotmail.com>

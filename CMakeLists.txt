# for help, see
#   http://www.cmake.org/cmake/help/v3.0
#   www.cmake.org/cmake/help/v3.0/manual/cmake-buildsystem.7.html
#   http://www.cmake.org/cmake/help/v3.0/manual/cmake-properties.7.html
#   http://www.cmake.org/pipermail/cmake/2005-September/007204.html
#
#   use 'cmake -LAH' for a CMake project to see values of variables
#   including CMake code: http://www.cmake.org/cmake/help/v3.0/command/include.html
#   
#
# CMake variabel types: normal and cache.
# normal variables are created when running a CMakeLists.txt
# cache variables are defined for a CMake project. typically a project option.
# cache variables can be defined by hand in CMakeCache.txt,
# or easily edited with 'cmake-gui'.
#
cmake_minimum_required( VERSION 3.0 )

# open-forest
project( open-forest )

# set version
set( BATB_VERSION_MAJOR  0 )
set( BATB_VERSION_MINOR  0 )
set( BATB_VERSION_STRING "${BATB_VERSION_MAJOR}.${BATB_VERSION_MINOR}" )


################################################################################
# set compiler flags
#
# FIXME: differentiate add_compile_options between each language
# see:   
#        http://stackoverflow.com/questions/25525047/cmake-generator-expression-differentiate-c-c-code
#        http://stackoverflow.com/questions/23995019/what-is-the-modern-method-for-setting-general-compile-flags-in-cmake
#     
# temporary fix: look at linker language for each target
add_compile_options( -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function )
# C++ specific compiler options
add_compile_options( "$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-std=c++11>" ) 
add_compile_options( "$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-Wno-non-template-friend>" )
# C specific compiler options
#add_compile_options( "$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,C>:>" )

# the above does not work, use this instead:
#set( CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend" )
#set( CMAKE_C_FLAGS "-g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function" ) 

# currently, we invoke (typically, by 'build.sh') cmake with -DCMAKE_BUILD_TYPE=None
# because we got too much compiler warnings without, since Ogre sets the build type to RelWithDebInfo.
# later, we may actually rely on the default OGRE setting "RelWithDebInfo", since
# OGRE may be too slow (release build of OGRE can increase performance a lot, according to doc).


################################################################################
# options for building this project
option( BATB_BUILD      "Build OpenForest"                            ON  )
option( BATB_BUILD_OLD  "Build old-BATB part into OpenForest"         ON  )
option( BATB_INSTALL    "Install OpenForest"                          OFF )


################################################################################
# set folders 

set( MAIN_SOURCE_DIR          "${CMAKE_SOURCE_DIR}/source" )
set( LIBS_SOURCE_DIR          "${CMAKE_SOURCE_DIR}/libs" )


################################################################################
# find library GLEW
find_package( GLEW )
if ( NOT GLEW_FOUND )
    message( FATAL_ERROR "GLEW not found (open-forest depends on this library)" )
endif()

################################################################################
# build library 'glfw'
#
set( GLFW_BUILD_EXAMPLES    OFF CACHE BOOL "" FORCE )   # we do not want to build examples
set( GLFW_BUILD_DOCS        OFF CACHE BOOL "" FORCE )   # we do not want to build documentation
set( GLFW_BUILD_TESTS       OFF CACHE BOOL "" FORCE )   # we do not want to build tests 
set( GLFW_INSTALL           OFF CACHE BOOL "" FORCE )   # we do not want to install GLFW
add_subdirectory( ${LIBS_SOURCE_DIR}/glfw )


################################################################################
# build library 'my-tinyxml2'. this exports the necessary include directories
add_subdirectory( "${CMAKE_SOURCE_DIR}/cmake/libs/tinyxml2" )

################################################################################
# build library 'my-nanovg'. this exports the necessary include directories
add_subdirectory( "${CMAKE_SOURCE_DIR}/cmake/libs/nanovg" )

################################################################################
# build library 'my-turbobadger'. this exports the necessary include directories
add_subdirectory( "${CMAKE_SOURCE_DIR}/cmake/libs/turbobadger" )

################################################################################
# build library 'yaml-cpp'.
set( YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE )   # disable "testing and parse tools"
add_subdirectory( ${LIBS_SOURCE_DIR}/yaml-cpp )

################################################################################
# build Ogre
# see http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Building+Ogre+With+CMake
# also, 'cmake -LAH' can be used to lists CMake-values for the Ogre CMake project
#
# NOTE: if CMAKE_BUILD_TYPE not set, the Ogre subdirectory will set this to RelWithDebInfo.
#       see the following snippet of the Ogre CMakeLists.txt:
#
#         if (CMAKE_BUILD_TYPE STREQUAL "")
#           # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
#           # differentiation between debug and release builds.
#           set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
#         endif ()
#
#set( CMAKE_BUILD_TYPE "None" CACHE STRING "" FORCE )                    # prevent "RelWithDebInfo" 
set( OGRE_BUILD_COMPONENT_OVERLAY             ON CACHE BOOL "" FORCE)   # overlay component
set( OGRE_BUILD_COMPONENT_PAGING              ON CACHE BOOL "" FORCE)   # paging component
set( OGRE_BUILD_COMPONENT_PROPERTY            ON CACHE BOOL "" FORCE)   # property component
set( OGRE_BUILD_COMPONENT_RTSSHADERSYSTEM     ON CACHE BOOL "" FORCE)   # rtsshader component
set( OGRE_BUILD_COMPONENT_TERRAIN             ON CACHE BOOL "" FORCE)   # terrain component
set( OGRE_BUILD_COMPONENT_VOLUME              ON CACHE BOOL "" FORCE)   # volume component
set( OGRE_BUILD_COMPONENT_BSP                 ON CACHE BOOL "" FORCE)   # bsp component
set( OGRE_BUILD_PLUGIN_CG                     ON CACHE BOOL "" FORCE)   # Cg plugin
set( OGRE_BUILD_PLUGIN_OCTREE                 ON CACHE BOOL "" FORCE)   # octree plugin
set( OGRE_BUILD_PLUGIN_PCZ                    ON CACHE BOOL "" FORCE)   # PCZ plugin
set( OGRE_BUILD_PLUGIN_PFX                    ON CACHE BOOL "" FORCE)   # PFX plugin
set( OGRE_BUILD_RENDERSYSTEM_GL               ON CACHE BOOL "" FORCE)   # GL-rendering
set( OGRE_BUILD_RENDERSYSTEM_GL3Plus          ON CACHE BOOL "" FORCE)   # GL3+-rendering
#set( OGRE_BUILD_RENDERSYSTEM_GLES2            ON CACHE BOOL "" FORCE)   # GLES2-rendering.
#set( OGRE_CONFIG_THREADS 2                    ON CACHE BOOL "" FORCE)   # thread setting 2, default. see doc...
#set( OGRE_FULL_RPATH                          ON CACHE BOOL "" FORCE)   # compile RPATH. FIXME.
set( OGRE_BUILD_SAMPLES                       OFF CACHE BOOL "" FORCE)  # build OGRE samples
set( OGRE_BUILD_TOOLS                         ON CACHE BOOL "" FORCE)   # build OGRE tools
#set( OGRE_CONFIG_DOUBLE                       ON CACHE BOOL "" FORCE)   # use 'double' as fp type
set( OGRE_INSTALL_DOCS                        OFF CACHE BOOL "" FORCE)  # install documentation
set( OGRE_INSTALL_SAMPLES                     OFF CACHE BOOL "" FORCE)  # install samples
set( OGRE_INSTALL_TOOLS                       OFF CACHE BOOL "" FORCE)  # install tools
set( OGRE_STATIC                              OFF CACHE BOOL "" FORCE)  # build OGRE as a static lib.
add_subdirectory( "${LIBS_SOURCE_DIR}/ogre" )


################################################################################
# build library 'ALURE'
#
# examples can be disabled by BUILD_EXAMPLES and INSTALL_EXAMPLES, according
# to alure doc.
#
# available codecs (needs libraries) :
#   Vorbis (needs OGG), FLAC (needs OGG), Opus (needs OGG), SndFile, MPG123, DUMB
# PhysFS resource streaming available
set( ALURE_INSTALL           OFF CACHE BOOL "" FORCE )   # we do not want to install ALURE
set( ALURE_USE_RTTI          OFF CACHE BOOL "" FORCE ) 
add_subdirectory( ${LIBS_SOURCE_DIR}/alure )
# alure2_s does not expose libs, lets add'em
list( APPEND CMAKE_MODULE_PATH "${LIBS_SOURCE_DIR}/alure/cmake" )
find_package( OpenAL REQUIRED )
list( APPEND ALURE_LIBRARIES ${OPENAL_LIBRARY} )
find_package( Ogg )
find_package( Vorbis )
find_package( FLAC )
find_package( Opus )
find_package( SndFile )
find_package( MPG123 )
find_package( DUMB )
#find_package( PHYSFS )
# EXERCISE: make this work:
#set( ALURE_LIBRARIES  $<$<STREQUAL:TRUE,${OGG_FOUND}>:"${OGG_LIBRARIES}"> 
#                      $<$<STREQUAL:TRUE,${VORBIS_FOUND}>:"${VORBIS_LIBRARIES}"> 
#                      $<$<STREQUAL:TRUE,${FLAC_FOUND}>:"${FLAC_LIBRARIES}"> 
#                      $<$<STREQUAL:TRUE,${OPUS_FOUND}>:"${OPUS_LIBRARIES}"> 
#                      $<$<STREQUAL:TRUE,${SNDFILE_FOUND}>:"${SNDFILE_LIBRARIES}"> 
#                      $<$<STREQUAL:TRUE,${MPG123_FOUND}>:"${MPG123_LIBRARIES}"> 
#                      #$<$<STREQUAL:TRUE,${DUMB_FOUND}>:"${DUMB_LIBRARIES}">
#                      $<$<STREQUAL:TRUE,${PHYSFS_FOUND}>:"${PHYSFS_LIBRARIES}"> 
#                      )
if ( OGG_FOUND )
list( APPEND ALURE_LIBRARIES ${OGG_LIBRARIES} )
endif()
if ( VORBIS_FOUND )
list( APPEND ALURE_LIBRARIES ${VORBIS_LIBRARIES} )
endif()
if ( FLAC_FOUND )
list( APPEND ALURE_LIBRARIES ${FLAC_LIBRARIES} )
endif()
if ( OPUS_FOUND )
list( APPEND ALURE_LIBRARIES ${OPUS_LIBRARIES} )
endif()
if ( SNDFILE_FOUND )
list( APPEND ALURE_LIBRARIES ${SNDFILE_LIBRARIES} )
endif()
if ( MPG123_FOUND )
list( APPEND ALURE_LIBRARIES ${MPG123_LIBRARIES} )
endif()
if ( DUMB_FOUND )
list( APPEND ALURE_LIBRARIES ${DUMB_LIBRARIES} )
endif()
if ( PHYSFS_FOUND )
list( APPEND ALURE_LIBRARIES ${PHYSFS_LIBRARIES} )
endif()



################################################################################
# lib-file
add_library(  lib-file STATIC 
              "${MAIN_SOURCE_DIR}/file.cpp"
           )
target_include_directories( lib-file PUBLIC "${MAIN_SOURCE_DIR}" )

################################################################################
# lib-env
add_library(  lib-env STATIC
              "${MAIN_SOURCE_DIR}/env.cpp"
              "${MAIN_SOURCE_DIR}/env/screen.cpp"
              "${MAIN_SOURCE_DIR}/env/frame.cpp"
           )
target_include_directories( lib-env PUBLIC "${MAIN_SOURCE_DIR}" )
target_include_directories( lib-env PUBLIC "${LIBS_SOURCE_DIR}/yaml-cpp/include" )
target_link_libraries( lib-env yaml-cpp )


################################################################################
# define batb sources
set(  BATB_SOURCES
      "${MAIN_SOURCE_DIR}/main.cpp"
      "${MAIN_SOURCE_DIR}/batb/BATB.cpp"
      # BATB core:
      "${MAIN_SOURCE_DIR}/batb/log/Log.cpp"
      "${MAIN_SOURCE_DIR}/batb/value/Value.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/Keys.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/KeyClicker.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/KeyPointer.cpp"
      "${MAIN_SOURCE_DIR}/batb/gui/GUI.cpp"
      "${MAIN_SOURCE_DIR}/batb/gui/tb_system.cpp"
      # BATB non-core:
      "${MAIN_SOURCE_DIR}/batb/ogre/OGRE.cpp"
      "${MAIN_SOURCE_DIR}/batb/ogre/GLContextGLFW.cpp"
      "${MAIN_SOURCE_DIR}/batb/al/AL.cpp"
      #"${MAIN_SOURCE_DIR}/batb/forest/Forest.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/World.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/Run.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRun.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRunBegin.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRunEnd.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRunMain.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRunOld.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/gui/GUIMain.cpp"
      # helpers
      "${MAIN_SOURCE_DIR}/batb/gl.cpp"             
      # library implementations:
      "${MAIN_SOURCE_DIR}/nanovg.cpp"             
      # tmp:
      "${MAIN_SOURCE_DIR}/batb/tmp/nanovg/demo.c"
      "${MAIN_SOURCE_DIR}/batb/tmp/nanovg/perf.c"
      "${MAIN_SOURCE_DIR}/batb/tmp/nanovg.cpp"
      "${MAIN_SOURCE_DIR}/batb/tmp/ogre/PerlinNoiseTerrainGenerator.cpp"
      "${MAIN_SOURCE_DIR}/batb/tmp/ogre.cpp"
      "${MAIN_SOURCE_DIR}/batb/tmp/al.cpp"
      
   )

################################################################################
# define old sources
#

set(  BATB_SOURCES_OLD 
      "${MAIN_SOURCE_DIR}/batb/old/old.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/calcalt.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/client.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/common.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/control.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/course.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/database.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgCommon.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgCourse.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgEvent.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgHost1.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgHost2.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgJoin1.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgJoin2.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgMap.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgMenu.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgOptions.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgOrienteer.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgPlayers.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgResult.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgSelect.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgSingle.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgStack.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/dlgTerrain.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/event.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/eventproxy.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/fdManager.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/forest.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/global.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/mainWindow.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/map.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/mapproxy.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/md5.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/mesh.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/message.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/noise.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/orienteer.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/oriproxy.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/point.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/random.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/run.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/server.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/shader.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/terrain.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/terrainproxy.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/texture.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/util.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/windowMessage.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/xmlParser.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/xmountains.cpp"
      "${MAIN_SOURCE_DIR}/batb/old/mainWindow.cpp"
   )


################################################################################
# build executable 'open-forest'
################################################################################

if ( BATB_BUILD )


    # old-BATB
    if( BATB_BUILD_OLD )
        list( APPEND BATB_SOURCES ${BATB_SOURCES_OLD} )
    endif( BATB_BUILD_OLD )


    # create executable target 'open-forest'
    add_executable( open-forest ${BATB_SOURCES} ) 

    # define target properties
    set_target_properties( open-forest PROPERTIES OUTPUT_NAME "open-forest" )

    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/glm" )
    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/tmp/nanovg" )

    # GLEW
    list( APPEND BATB_LIBRARIES ${GLEW_LIBRARIES} )
    target_include_directories( open-forest PRIVATE ${GLEW_INCLUDE_DIRS} )

    # GLFW
    list( APPEND BATB_LIBRARIES glfw ${GLFW_LIBRARIES} )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/glfw/include" )

    # my-tinyxml2
    list( APPEND BATB_LIBRARIES my-tinyxml2 )

    # my-nanovg
    list( APPEND BATB_LIBRARIES my-nanovg )

    # my-turbobadger
    list( APPEND BATB_LIBRARIES my-turbobadger )
    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/gui" ) # point to _our_ tb_config.h

    # yaml-cpp
    list( APPEND BATB_LIBRARIES yaml-cpp )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/yaml-cpp/include" )

    # Ogre
    list( APPEND BATB_LIBRARIES OgreMain )
    list( APPEND BATB_LIBRARIES OgreTerrain )
    list( APPEND BATB_LIBRARIES OgrePaging )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/OgreMain/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Paging/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Terrain/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre" )
    # "${LIBS_SOURCE_DIR}/ogre/RenderSystems/GL/include" <- must not be included, it overrides glew.h !!!
    if ( OGRE_BUILD_RENDERSYSTEM_GL )
        list( APPEND BATB_LIBRARIES RenderSystem_GL )
    endif()
    if ( OGRE_BUILD_RENDERSYSTEM_GL3Plus )
        #list( APPEND BATB_LIBRARIES RenderSystem_GL3Plus ) #GL3Plus is not working for me yet...
    endif()

    # ALURE
    list( APPEND BATB_LIBRARIES alure2_s ${ALURE_LIBRARIES} )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/alure/include" )  # NOTE: this adds alc.h, etc. from alure,
                                                                                          #       if #include "alc.h" and not
                                                                                          #       #include "AL/alc.h", see lib source

    # lib-file
    list( APPEND BATB_LIBRARIES lib-file )

    # lib-env
    list( APPEND BATB_LIBRARIES lib-env )

    # old-BATB
    if( BATB_BUILD_OLD )
        list( APPEND BATB_LIBRARIES -lplibpuaux -lplibpu -lplibfnt -lplibsg -lplibul -lGLEW -lGL -lGLU -lglut -lm -lc ) # FIXME: find package plib
        target_compile_definitions( open-forest PRIVATE PU_USE_NONE ) # remove GLUT from plib
        target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/old" )
    endif( BATB_BUILD_OLD )


    # write our CMake variables into code
    configure_file( "${MAIN_SOURCE_DIR}/definitions.hpp.in" "${MAIN_SOURCE_DIR}/definitions.hpp" )


    # link libraries to our executable target
    target_link_libraries( open-forest ${BATB_LIBRARIES} )

endif()


################################################################################
# Install open-forest
################################################################################

if ( BATB_INSTALL )
    message( "install not implemented..." )
    # ...
endif()




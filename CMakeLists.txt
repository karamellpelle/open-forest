# for help, see
#   http://www.itk.org/Wiki/CMake_FAQ#Writing_CMakeLists.txt
#   http://www.cmake.org/cmake/help/cmake_tutorial.html
#   http://www.cmake.org/Wiki/CMake_Useful_Variables
#   http://binglongx.wordpress.com/tag/cmake_build_type/
#   'cmake -LAH' in CMakeLists.txt directory to see varible values
#   http://www.cmake.org/pipermail/cmake/2005-September/007204.html
#   
#
#   CMake variabel types: normal and cache. see: 
#   http://www.cmake.org/cmake/help/v2.8.9/cmake.html#command%3aset
#   cache variables are variables persisting each cmake invocation. they
#   are defined for each cmake invocation (i.e. build folder for the main project)
#   these cache variables values can (later) be defined by hand in CMakeCache.txt
#                   
#   FIXME:
#   * include_directories -> target_include_directories
#   * CMAKE_BUILD_TYPE and compiler flags (there is a new command, compiler_add_flags or something)
cmake_minimum_required( VERSION 2.6 )
project ( open-forest )

# set version
set(BATB_VERSION_MAJOR 0)
set(BATB_VERSION_MAJOR 1)

# set compiler flags
# FIXME: these flags are only used when the cache variable CMAKE_BUILD_TYPE:STRING=None
#        (or undefined)
#        instead define our different flags in CMAKE_CXX_FLAGS_*/CMAKE_C_FLAGS_* 
#        (where * is one of DEBUG, RELEASE, RELWITHDEBINFO, MINSIZEREL)
#        below, we set CMAKE_BUILD_TYPE:STRING=None, to prevent Ogre setting this
#        to RelWithDebInfo!
#        later, we may actually rely on the default OGRE setting "RelWithDebInfo", since
#        OGRE may be too slow (release build of OGRE can increase performance a lot, according to doc).
# TODO:  use 'add_compile_options', available in CMake 2.8.12 (www.stackoverflow.com/questions/15100351)
set( CMAKE_CXX_FLAGS      "-std=c++11 -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend" )
set( CMAKE_C_FLAGS        "-g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function" )

# set folders 
set( MAIN_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/source )
set( LIBS_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs )
set( CUSTOM_CMAKELISTS_DIR    ${CMAKE_SOURCE_DIR}/cmake/libs )


################################################################################
# find library GLEW
find_package( GLEW )
if ( NOT GLEW_FOUND )
    message( FATAL_ERROR "GLEW not found (open-forest depends on this library)" )
endif()

################################################################################
# build library 'glfw'
#
# there seems to be a bug, GLFW_LIBRARIES does not contain libXcursor.
# bug may go away if uncomment these 3 disables below, first time (??)
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   # we do not want to build examples
set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )      # we do not want to build documentation
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )     # we do not want to build tests 
set( GLFW_INSTALL OFF CACHE BOOL "" FORCE )     # we do not want to build tests 
set( GLFW_SOURCE_DIR ${LIBS_SOURCE_DIR}/glfw )
add_subdirectory( ${GLFW_SOURCE_DIR} )


################################################################################
# build library 'custom-tinyxml2'
# this will create
#   custom-tinyxml2                   : target
#   CUSTOM_TINYXML2_INCLUDE_DIRS      : include directory for library
#   CUSTOM_TINYXML2_LIBRARIES         : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/tinyxml2 )

################################################################################
# build library 'custom-nanovg'
# this will create
#   custom-nanovg                     : target
#   CUSTOM_NANOVG_INCLUDE_DIRS        : include directory for library
#   CUSTOM_NANOVG_LIBRARIES           : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/nanovg )

################################################################################
# build library 'custom-turbobadger'
# this will create
#   custom-turbobadger                : target
#   CUSTOM_TURBOBADGER_INCLUDE_DIRS   : include directories for library
#   CUSTOM_TURBOBADGER_LIBRARIES      : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/turbobadger )

################################################################################
# build library 'yaml-cpp'
set( YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE) # disable "testing and parse tools"
add_subdirectory( ${LIBS_SOURCE_DIR}/yaml-cpp )

################################################################################
# build OGRE
# see http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Building+Ogre+With+CMake
# also, 'cmake -LAH' lists CMake-values in directory
#
# other settings too look at later:
#   * DDS/ETC codec?
#   * CONFIG_ENABLE_GL_STATE_CACHE_SUPPORT?
#   * CONFIG_ENABLE_PVRTC?
#
# NOTE: if CMAKE_BUILD_TYPE not set, the Ogre subdirectory will set this to RelWithDebInfo.
#       actual code:
#       if (CMAKE_BUILD_TYPE STREQUAL "")
#         # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
#         # differentiation between debug and release builds.
#         set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
#       endif ()
#
#       below we forced this value to be be None, but now we should define this value when building (cmake -DCMAKE_BUILD_TYPE)
#       this is done by 'build.sh'!

#set( CMAKE_BUILD_TYPE "None" CACHE STRING "" FORCE )                # prevent "RelWithDebInfo" pollution of main project, open-forest.
set( OGRE_BUILD_COMPONENT_OVERLAY ON CACHE BOOL "" FORCE)           # overlay component
set( OGRE_BUILD_COMPONENT_PAGING ON CACHE BOOL "" FORCE)            # paging component
set( OGRE_BUILD_COMPONENT_PROPERTY ON CACHE BOOL "" FORCE)          # property component
set( OGRE_BUILD_COMPONENT_RTSSHADERSYSTEM ON CACHE BOOL "" FORCE)   # rtsshader component
set( OGRE_BUILD_COMPONENT_TERRAIN ON CACHE BOOL "" FORCE)           # terrain component
set( OGRE_BUILD_COMPONENT_VOLUME ON CACHE BOOL "" FORCE)            # volume component
set( OGRE_BUILD_COMPONENT_BSP ON CACHE BOOL "" FORCE)               # bsp component
set( OGRE_BUILD_PLUGIN_CG ON CACHE BOOL "" FORCE)                   # Cg plugin
set( OGRE_BUILD_PLUGIN_OCTREE ON CACHE BOOL "" FORCE)               # octree plugin
set( OGRE_BUILD_PLUGIN_PCZ ON CACHE BOOL "" FORCE)                  # PCZ plugin
set( OGRE_BUILD_PLUGIN_PFX ON CACHE BOOL "" FORCE)                  # PFX plugin
set( OGRE_BUILD_RENDERSYSTEM_GL ON CACHE BOOL "" FORCE)             # GL-rendering
set( OGRE_BUILD_RENDERSYSTEM_GL3Plus ON CACHE BOOL "" FORCE)        # GL3+-rendering
#set( OGRE_BUILD_RENDERSYSTEM_GLES2 ON CACHE BOOL "" FORCE)          # GLES2-rendering.
#set( OGRE_CONFIG_THREADS 2            ON CACHE BOOL "" FORCE)       # thread setting 2, default. see doc...
#set( OGRE_FULL_RPATH                  ON CACHE BOOL "" FORCE)       # compile RPATH. FIXME.
set( OGRE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)                    # build OGRE samples
set( OGRE_BUILD_TOOLS ON CACHE BOOL "" FORCE)                       # build OGRE tools
#set( OGRE_CONFIG_DOUBLE ON CACHE BOOL "" FORCE)                     # use 'double' as fp type
set( OGRE_INSTALL_DOCS OFF OFF CACHE BOOL "" FORCE)                 # install documentation
set( OGRE_INSTALL_SAMPLES OFF OFF CACHE BOOL "" FORCE)              # install samples
set( OGRE_INSTALL_TOOLS OFF CACHE BOOL "" FORCE)                    # install tools
set( OGRE_STATIC OFF CACHE BOOL "" FORCE)                           # build OGRE as a static lib.

add_subdirectory( "${LIBS_SOURCE_DIR}/ogre" )


################################################################################
# set additional include directories
#
include_directories( "${MAIN_SOURCE_DIR}" )
include_directories( "${LIBS_SOURCE_DIR}/glm" )
include_directories( "${BATB_SOURCE_DIR}/tmp/nanovg" )

# set CMake working directory
set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build )


# options and default settings
option( BATB_BUILD   "Build BATB" ON)
option( BATB_INSTALL "Install BATB" OFF)



################################################################################
# define .cpp files


# define file sources
set( FILE_SOURCES
      "${MAIN_SOURCE_DIR}/file.cpp"
)

# define env sources
set( ENV_SOURCES
      "${MAIN_SOURCE_DIR}/env.cpp"
      "${MAIN_SOURCE_DIR}/env/screen.cpp"
      "${MAIN_SOURCE_DIR}/env/frame.cpp"
)

# define batb sources
set( BATB_SOURCES
     
      
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

      # add tmp for now
      "${MAIN_SOURCE_DIR}/batb/tmp/nanovg/demo.c"
      "${MAIN_SOURCE_DIR}/batb/tmp/nanovg/perf.c"
      "${MAIN_SOURCE_DIR}/batb/tmp/nanovg.cpp"

)

################################################################################
# define old sources
#

set( BATB_OLD_SOURCES 
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
# build executable 'batb'
################################################################################


if ( BATB_BUILD )

    # define MAIN
    set( MAIN_LIBRARIES "" )
    set( MAIN_SOURCES "${MAIN_SOURCE_DIR}/main.cpp" )

    
    # GLEW
    include_directories( APPEND MAIN_LIBRARIES ${GLEW_INCLUDE_DIRS} )
    list( APPEND MAIN_LIBRARIES ${GLEW_LIBRARIES} )

    # GLFW
    include_directories( "${GLFW_SOURCE_DIR}/include" )
    list( APPEND MAIN_LIBRARIES glfw ${GLFW_LIBRARIES} )

    # tinyxml2
    include_directories( ${CUSTOM_TINYXML2_INCLUDE_DIRS} )
    list( APPEND MAIN_LIBRARIES custom-tinyxml2 ${CUSTOM_TINYXML2_LIBRARIES} )

    # nanovg
    include_directories( ${CUSTOM_NANOVG_INCLUDE_DIRS} )
    list( APPEND MAIN_LIBRARIES custom-nanovg ${CUSTOM_NANOVG_LIBRARIES} )
    list( APPEND MAIN_SOURCES "${MAIN_SOURCE_DIR}/nanovg.cpp" ) # also add our implementation source file

    # turbobadger
    include_directories( ${CUSTOM_TURBOBADGER_INCLUDE_DIRS} )
    include_directories( ${MAIN_SOURCE_DIR}/batb/gui )          # also add our tb_config.h implementation
    list( APPEND MAIN_LIBRARIES custom-turbobadger ${CUSTOM_TURBOBADGER_LIBRARIES} )

    # yaml-cpp
    include_directories( ${LIBS_SOURCE_DIR}/yaml-cpp/include )
    list( APPEND MAIN_LIBRARIES yaml-cpp )

    # Ogre
    include_directories( "${LIBS_SOURCE_DIR}/ogre/include" )
    include_directories( "${LIBS_SOURCE_DIR}/ogre/OgreMain/include" )
    list( APPEND MAIN_LIBRARIES OgreMain )

    # OLD
# ifdef "build old"
    add_definitions( -DPU_USE_NONE ) # remove GLUT from plib
    include_directories( "${MAIN_SOURCE_DIR}/batb/old" )
    set( BATB_OLD_LIBRARIES -lplibpuaux -lplibpu -lplibfnt -lplibsg -lplibul -lGLEW -lGL -lGLU -lglut -lm -lc)

    #include_directories( "/home/karamellpelle/Tmp/plib-install/include" )
    #set( KLIB "/home/karamellpelle/Tmp/plib-install/lib" )
    #set( BATB_OLD_LIBRARIES "${KLIB}/libplibpuaux.a" "${KLIB}/libplibpu.a" "${KLIB}/libplibfnt.a" "${KLIB}/libplibsg.a" "${KLIB}/libplibul.a" -lGLEW -lGL -lGLU -lm -lc)

    list( APPEND MAIN_LIBRARIES ${BATB_OLD_LIBRARIES} )
    list( APPEND MAIN_SOURCES ${BATB_OLD_SOURCES} )
# endif

    # batb
    list( APPEND MAIN_LIBRARIES ${BATB_LIBRARIES} )
    list( APPEND MAIN_SOURCES ${BATB_SOURCES} ${FILE_SOURCES} ${ENV_SOURCES})



    ###########################################################
    # create executable target 'open-forest'
    add_executable( open-forest ${MAIN_SOURCES} ) 

    # define target properties
    set_target_properties( open-forest PROPERTIES
                           OUTPUT_NAME "open-forest"
                         )
   
    # libraries to target
    target_link_libraries( open-forest ${MAIN_LIBRARIES} )

endif()



### INSTALL ###
if ( BATB_INSTALL )
    message( "install not implemented..." )
    #install( TARGETS batb DESTINATION bin)
    #install( FILES   "data/" DESTINATION bin)
endif()




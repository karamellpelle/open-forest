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
# interesting variables (https://cmake.org/Wiki/CMake_Useful_Variables):
#
# CMAKE_BINARY_DIR:         top level out-of-source build folder (i.e. ./build)
# CMAKE_CURRENT_BINARY_DIR: sub-CMakeLists.txt out-of-source build folder (for example ./build/libs/glfw)
# CMAKE_SOURCE_DIR:         top level source folder (i.e. ./)
# CMAKE_CURRENT_SOURCE_DIR: sub-CMakeLists.txt source folder (for example ./libs/glfw)
# CMAKE_MODULE_PATH:        search for cmake includes and modules first here
# EXECUTABLE_OUTPUT_PATH:   put executables here (instead of build folder)
# LIBRARY_OUTPUT_PATH:      put libraries here (instead of build folder)
# PROJECT_SOURCE_DIR:       top CMakeLists.txt for project (for example a for library with sub-CMakeLists.txt
# PROJECT_BINARY_DIR:       the corresponding out-of-source build folder for PROJECT_SOURCE_DIR

cmake_minimum_required( VERSION 3.3 )

# tell CMake to force c++14. -std=c++14 removed from original ogre and alure CMakeLists.txt
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# set up RPATH (paths used for dynamic linkage)
# https://cmake.org/Wiki/CMake_RPATH_handling#CMake_and_the_RPATH
# https://blog.kitware.com/upcoming-in-cmake-2-8-12-osx-rpath-support/
set( CMAKE_MACOSX_RPATH TRUE )
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
################################################################################
# this is more interesting later when we implement install (i.e. creating a 
# bundle on macOS)in this CMakeLists!  
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")
################################################################################




################################################################################
# open-forest
################################################################################

project( open-forest )

################################################################################
# set compiler flags
#
# see:   
#        http://stackoverflow.com/questions/25525047/cmake-generator-expression-differentiate-c-c-code
#        http://stackoverflow.com/questions/23995019/what-is-the-modern-method-for-setting-general-compile-flags-in-cmake
#        http://marshall.calepin.co/llvmclang-and-standard-libraries-on-mac-os-x.html

# set flags based on compiler
# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
# http://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
if ( ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") )
  # using Clang
  # warnings: http://clang.llvm.org/docs/DiagnosticsReference.html
  set( flags_cpp -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-deprecated-declarations -Wno-overloaded-virtual -Wno-inconsistent-missing-override -Wno-unused-private-field )
  set( flags_c -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set( flags_cpp -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend -Wno-deprecated-declarations )
  set( flags_c -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function )
else()
  # for now, use same as GCC
  set( flags_cpp -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend -Wno-deprecated-declarations )
  set( flags_c -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function )
endif()

# generator expressions:
# http://stackoverflow.com/questions/15100351/changing-cmake-cxx-flags-in-project
# http://stackoverflow.com/questions/25525047/cmake-generator-expression-differentiate-c-c-code#35361099
add_compile_options( "$<$<COMPILE_LANGUAGE:CXX>:${flags_cpp}>" )
add_compile_options( "$<$<COMPILE_LANGUAGE:C>:${flags_c}>" )

# currently, we invoke (typically, by 'build.sh') cmake with -DCMAKE_BUILD_TYPE=None
# because we got too much compiler warnings without, since Ogre sets the build type to RelWithDebInfo.
# later, we may actually rely on the default OGRE setting "RelWithDebInfo", since
# OGRE may be too slow (release build of OGRE can increase performance a lot, according to doc).



################################################################################
# options for building this project

option( BATB_BUILD      "Build OpenForest executable"                 ON  )
option( BATB_BUILD_OLD  "Build old code into open-forest"             ON  )
option( BATB_INSTALL    "Install OpenForest"                          OFF )


set( MAIN_SOURCE_DIR          "${CMAKE_SOURCE_DIR}/source" )
set( LIBS_SOURCE_DIR          "${CMAKE_SOURCE_DIR}/libs" )
set( LIBS_BINARY_DIR          "${CMAKE_BINARY_DIR}/libs" )

# where we can put custom FindXXX.cmake
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

# set platform
# TODO: linux, etc
if ( UNIX )
  if ( APPLE ) # FIXME: and not iOS
      set( BATB_BUILD_PLATFORM_MACOS ON ) 
  else()
      set( BATB_BUILD_PLATFORM_LINUX ON )
  endif()
endif()


# set version
set( BATB_VERSION_MAJOR  0 )
set( BATB_VERSION_MINOR  0 )
set( BATB_VERSION_STRING "${BATB_VERSION_MAJOR}.${BATB_VERSION_MINOR}" )

################################################################################
# find package Boost
# todo: as git and cmake submodule
find_package(Boost REQUIRED)

################################################################################
# find library GLEW
# todo: as git and cmake submodule
find_package( GLEW REQUIRED )

################################################################################
# find library OpenGL
find_package( OpenGL REQUIRED )

################################################################################
# build library 'glfw'
set( GLFW_BUILD_EXAMPLES    OFF CACHE BOOL "" FORCE )   # we do not want to build examples
set( GLFW_BUILD_DOCS        OFF CACHE BOOL "" FORCE )   # we do not want to build documentation
set( GLFW_BUILD_TESTS       OFF CACHE BOOL "" FORCE )   # we do not want to build tests 
set( GLFW_INSTALL           OFF CACHE BOOL "" FORCE )   # we do not want to install GLFW
add_subdirectory( ${LIBS_SOURCE_DIR}/glfw )


################################################################################
# build library 'my-tinyxml2'. this exports the necessary include directories
add_subdirectory( "${CMAKE_SOURCE_DIR}/cmake/libs/tinyxml2" )


################################################################################
# build library 'my-turbobadger'. this exports the necessary include directories
# NOTE my-turbobadger -> TurboBadgerLib
add_subdirectory( "${CMAKE_SOURCE_DIR}/cmake/libs/turbobadger" )
## build library TurboBadgerLib
#set( TB_BUILD_DEMO OFF)
#add_subdirectory( ${LIBS_SOURCE_DIR}/turbobadger )


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
# TODO: 
#   * OGRE_CONFIG_ENABLE_GL_STATE_CACHE_SUPPORT ?
set( CMAKE_BUILD_TYPE "None" CACHE STRING "" FORCE )                    # prevent "RelWithDebInfo" 
set( OGRE_BUILD_COMPONENT_OVERLAY             ON CACHE BOOL "" FORCE)   # overlay component
set( OGRE_BUILD_COMPONENT_PAGING              ON CACHE BOOL "" FORCE)   # paging component
set( OGRE_BUILD_COMPONENT_PROPERTY            ON CACHE BOOL "" FORCE)   # property component
set( OGRE_BUILD_COMPONENT_RTSSHADERSYSTEM     ON CACHE BOOL "" FORCE)   # rtsshader componentCMAKE_CURRENT_SOURCE_DIR
set( OGRE_BUILD_COMPONENT_TERRAIN             ON CACHE BOOL "" FORCE)   # terrain component
set( OGRE_BUILD_COMPONENT_VOLUME              ON CACHE BOOL "" FORCE)   # volume component
set( OGRE_BUILD_COMPONENT_BSP                 ON CACHE BOOL "" FORCE)   # bsp component
#set( OGRE_BUILD_PLUGIN_CG                     ON CACHE BOOL "" FORCE)   # Cg plugin
set( OGRE_BUILD_PLUGIN_OCTREE                 ON CACHE BOOL "" FORCE)   # octree plugin
set( OGRE_BUILD_PLUGIN_PCZ                    ON CACHE BOOL "" FORCE)   # PCZ plugin
set( OGRE_BUILD_PLUGIN_PFX                    ON CACHE BOOL "" FORCE)   # PFX plugin
set( OGRE_BUILD_RENDERSYSTEM_GL               ON CACHE BOOL "" FORCE)   # GL-rendering
set( OGRE_BUILD_RENDERSYSTEM_GL3Plus          ON CACHE BOOL "" FORCE)   # GL3+-rendering # FIXME: does not seem to exists
#set( OGRE_BUILD_RENDERSYSTEM_GL3PLUS          ON CACHE BOOL "" FORCE)   # GL3+-rendering # FIXME: use this (capital letters)
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

# set release build for Ogre3D (should be faster)
# http://stackoverflow.com/questions/30985215/passing-variables-down-to-subdirectory-only 
# http://stackoverflow.com/questions/24460486/cmake-build-type-not-being-used-in-cmakelists-txt/24470998#24470998
set( CMAKE_BUILD_TYPE_COPY "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/libs/open-forest-ogre" )
set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_COPY} )


################################################################################
# alure

add_subdirectory( ${LIBS_SOURCE_DIR}/alure )

#list( APPEND ALURE_LIBRARIES alure2 ${OPENAL_LIBRARY} )
list( APPEND ALURE_LIBRARIES alure2 )
list( APPEND ALURE_INCLUDE_DIRS "${LIBS_SOURCE_DIR}/alure/include" )

#
#list( APPEND CMAKE_MODULE_PATH "${LIBS_SOURCE_DIR}/alure/cmake" )
#find_package( Ogg )
#find_package( Vorbis )
#find_package( FLAC )
#find_package( Opus )
#find_package( SndFile )
#find_package( MPG123 )
#find_package( DUMB )
#
## empty appends allowed
#list( APPEND ALURE_LIBRARIES ${OGG_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${VORBIS_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${FLAC_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${OPUS_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${SNDFILE_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${MPG123_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${DUMB_LIBRARIES} )
#list( APPEND ALURE_LIBRARIES ${PHYSFS_LIBRARIES} )


################################################################################


################################################################################
# define batb sources
set(  BATB_SOURCES
      "${MAIN_SOURCE_DIR}/BATB.cpp"
      "${MAIN_SOURCE_DIR}/BATB/AL.cpp"
      "${MAIN_SOURCE_DIR}/BATB/CourseDrawer.cpp"             
      "${MAIN_SOURCE_DIR}/BATB/Demo.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/BeginEvents.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Course.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/CourseCurve.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration/IterationDemo.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration/IterationDemoForest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Output.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/al.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/demo.c"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/perf.c"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/ogre.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/ogre/PerlinNoiseTerrainGenerator.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/other.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/workers.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/BeginEvents.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Control.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Iteration/IterationForest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/ModifyCamera.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/ModifyControlCamera.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/ModifyControlRunner.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/ModifyRunner.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Output.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Runner.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/StepDT.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Terrain.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Weather.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/World.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GL.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb_file_BATB.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb_system_BATB.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/widgets/TBProgressBarWidget.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyClicker.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyPointer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Log.cpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE.cpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE/GLContextGLFW.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Player.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/BeginEvents.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/TBConsole.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/cmd.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/cmd/cmd_easteregg.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/cmd/cmd_value.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRun.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunDemo.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunMain.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunMain/TBMain.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunOld.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunWork.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunWork/RunWorkTBWidget.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Notifier.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Notifier/TBNotify.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/World.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/workers.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Screen.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Time.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Value.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/values.cpp"
      "${MAIN_SOURCE_DIR}/BATB/tb.cpp"             
      "${MAIN_SOURCE_DIR}/Module.cpp"
      "${MAIN_SOURCE_DIR}/File.cpp"
      "${MAIN_SOURCE_DIR}/main.cpp"

      ## nanovg
      ## TODO: make cmake library
      "${CMAKE_SOURCE_DIR}/libs/nanovg/src/nanovg.c"

      ## demo:
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/demo.c"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/perf.c"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/ogre/PerlinNoiseTerrainGenerator.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/ogre.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/al.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/other.cpp"
      
   )


################################################################################
# define old sources
#

set(  BATB_SOURCES_OLD 
      "${MAIN_SOURCE_DIR}/BATB/old/old.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/calcalt.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/client.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/common.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/control.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/course.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/database.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgCommon.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgCourse.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgEvent.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgHost1.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgHost2.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgJoin1.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgJoin2.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgMap.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgMenu.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgOptions.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgOrienteer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgPlayers.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgResult.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgSelect.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgSingle.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgStack.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/dlgTerrain.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/event.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/eventproxy.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/fdManager.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/forest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/global.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/mainWindow.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/map.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/mapproxy.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/md5.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/mesh.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/message.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/noise.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/orienteer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/oriproxy.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/point.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/random.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/run.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/server.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/shader.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/terrain.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/terrainproxy.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/texture.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/util.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/windowMessage.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/xmlParser.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/xmountains.cpp"
      "${MAIN_SOURCE_DIR}/BATB/old/mainWindow.cpp"
   )


################################################################################
# build executable 'open-forest'
################################################################################

if ( BATB_BUILD )


    ############################################################################
    # old
    if( BATB_BUILD_OLD )
        find_package( Plib )
        if ( PLIB_FOUND )
            list( APPEND BATB_SOURCES ${BATB_SOURCES_OLD} )
        else()
            message( WARNING "Plib (needed for BATB_BUILD_OLD) not found. skipping build of old code" )
            set( BATB_BUILD_OLD OFF )
        endif()
    endif( BATB_BUILD_OLD )

    ############################################################################
    # create executable target 'open-forest'
    add_executable( open-forest ${BATB_SOURCES} ) 
    

    # define target properties
    set_target_properties( open-forest PROPERTIES OUTPUT_NAME "open-forest" )

    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/glm" )
    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/tmp/nanovg" )
    target_include_directories( open-forest PRIVATE "${CMAKE_SOURCE_DIR}/libs/nanovg/src" )

    # GLEW
    list( APPEND BATB_LIBRARIES ${GLEW_LIBRARIES} )
    target_include_directories( open-forest PRIVATE ${GLEW_INCLUDE_DIRS} )

    # GLFW
    list( APPEND BATB_LIBRARIES glfw ${GLFW_LIBRARIES} )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/glfw/include" )

    # my-tinyxml2
    list( APPEND BATB_LIBRARIES my-tinyxml2 )

    # my-turbobadger
    list( APPEND BATB_LIBRARIES my-turbobadger )
    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/gui" ) # point to _our_ tb_config.h
    if ( APPLE )
        target_compile_definitions( open-forest PUBLIC MACOSX ) # turbobadger needs MACOSX
    endif()

    # yaml-cpp
    list( APPEND BATB_LIBRARIES yaml-cpp )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/yaml-cpp/include" )

    # Ogre
    list( APPEND BATB_LIBRARIES OgreMain )
    list( APPEND BATB_LIBRARIES OgreTerrain )
    list( APPEND BATB_LIBRARIES OgrePaging )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/open-forest-ogre/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/open-forest-ogre/OgreMain/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/open-forest-ogre/Components/Paging/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/open-forest-ogre/Components/Terrain/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/open-forest-ogre" )
    target_include_directories( open-forest PRIVATE "${LIBS_BINARY_DIR}/open-forest-ogre/include" ) # we need the build generated OgreBuildSettings.h
    # "${LIBS_SOURCE_DIR}/ogre/RenderSystems/GL/include" <- must not be included, it overrides glew.h !!!


    if ( OGRE_BUILD_RENDERSYSTEM_GL )
        list( APPEND BATB_LIBRARIES RenderSystem_GL )
    endif()
    if ( OGRE_BUILD_RENDERSYSTEM_GL3Plus )
        #list( APPEND BATB_LIBRARIES RenderSystem_GL3Plus ) #GL3Plus is not working for me yet... FIXME: user capital letters
    endif()


    # link hazzle on macOS
    if ( APPLE )
        # fix a link error (add AGL)
        set_target_properties( open-forest PROPERTIES LINK_FLAGS "-framework AGL")
        # set where to look after dynamic libraries (where rpath points to (may be more than one; see doc))
        set_target_properties( open-forest PROPERTIES INSTALL_RPATH "@executable_path/lib/" )
    endif()

    # ALURE
    list( APPEND BATB_LIBRARIES ${ALURE_LIBRARIES} )
    target_include_directories( open-forest PRIVATE ${ALURE_INCLUDE_DIRS} )

    ############################################################################
    # old-BATB
    if( BATB_BUILD_OLD )
          list( APPEND BATB_LIBRARIES ${PLIB_LIBRARIES} )
          target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/old" )
          target_include_directories( open-forest PRIVATE ${PBLIB_INCLUDE_DIRS} )
          target_compile_definitions( open-forest PRIVATE PU_USE_NONE ) # remove GLUT from plib
    endif( BATB_BUILD_OLD )
    ############################################################################


    # write our CMake variables into code
    configure_file( "${MAIN_SOURCE_DIR}/definitions.hpp.in" "${MAIN_SOURCE_DIR}/definitions.hpp" )


    # link libraries to our executable target
    target_link_libraries( open-forest ${BATB_LIBRARIES} )

endif()


################################################################################
# Install open-forest
################################################################################
if ( BATB_INSTALL )
    message( "installation not implemented (yet)" )
    # ...
endif()
################################################################################


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
# CMAKE_BINARY_DIR:         top level out-of-source build folder, folder where 'cmake' was invoked (i.e. ./build/cmake)
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
# https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/RPATH-handling
# https://blog.kitware.com/upcoming-in-cmake-2-8-12-osx-rpath-support/
# https://www.mikeash.com/pyblog/friday-qa-2009-11-06-linking-and-install-names.html
set( CMAKE_MACOSX_RPATH TRUE )            # all dylib targets use @rpath for the directory portion of install_name 
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
# also, 'cmake -LAH' can be used to lists CMake-values for the Ogre CMake project
#
#set( CMAKE_BUILD_TYPE "None" CACHE STRING "" FORCE )                    # prevent "RelWithDebInfo" 

set( OGRE_BUILD_COMPONENT_BITES       OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_HLMS        ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_JAVA        OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_MESHLODGE   ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_OVERLAY     ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_PAGING      ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_PROPERTY    ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_PYTHON      OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_RTSHADERS   ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_TERRAIN     ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_COMPONENT_VOLUME      ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_DEPENDENCIES          OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_LIBS_AS_FRAMEWORKS    ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_BSP            ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_EXRCODEC       ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_FREEIMAGE      ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_OCTREE         ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_PCZ            ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_PFX            ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_PLUGIN_STBI           ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_RENDERSYSTEM_GL       ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_RENDERSYSTEM_GL3PLU   ON CACHE BOOL "" FORCE )
set( OGRE_BUILD_RENDERSYSTEM_GLES2    OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_SAMPLES               OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_TESTS                 OFF CACHE BOOL "" FORCE )
set( OGRE_BUILD_TOOLS                 ON CACHE BOOL "" FORCE )
set( OGRE_CONFIG_ENABLE_QUAD_BUFFER   OFF CACHE BOOL "" FORCE )
set( OGRE_CONFIG_THREADS              3 CACHE BOOL "" FORCE )
set( OGRE_CONFIG_THREAD_PROVIDER      std CACHE BOOL "" FORCE )
set( OGRE_COPY_DEPENDENCIES           ON CACHE BOOL "" FORCE )
set( OGRE_ENABLE_PRECOMPILED_HEADER   ON CACHE BOOL "" FORCE )
set( OGRE_GLSUPPORT_USE_EGL           OFF CACHE BOOL "" FORCE )
set( OGRE_INSTALL_CMAKE               ON CACHE BOOL "" FORCE )
set( OGRE_INSTALL_DEPENDENCIES        ON CACHE BOOL "" FORCE )
set( OGRE_INSTALL_DOCS                ON CACHE BOOL "" FORCE )
set( OGRE_INSTALL_SAMPLES             ON CACHE BOOL "" FORCE )
set( OGRE_INSTALL_TOOLS               OFF CACHE BOOL "" FORCE )
set( OGRE_RESOURCEMANAGER_STRICT      2 CACHE BOOL "" FORCE )
set( OGRE_STATIC                      OFF CACHE BOOL "" FORCE )

# set release build for Ogre3D (should be faster)
# http://stackoverflow.com/questions/30985215/passing-variables-down-to-subdirectory-only 
# http://stackoverflow.com/questions/24460486/cmake-build-type-not-being-used-in-cmakelists-txt/24470998#24470998
#set( CMAKE_BUILD_TYPE_COPY "${CMAKE_BUILD_TYPE}" )
#set( CMAKE_BUILD_TYPE "Release" )
#add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/libs/open-forest-ogre" )
#set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_COPY} )

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/libs/ogre" )


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
      "${MAIN_SOURCE_DIR}/BATB/Forest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Control.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Iteration/IterationForest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/WorldDrawer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/MapDrawer2D.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Sound.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/AimKeysController.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Runner.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/StepDT.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Terrain.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Terrain/TerrainPageProvider.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Weather.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/World.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/LoadWorld.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GL.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/tb_file_batb.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/tb_system_batb.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBRendererGL330.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBProgressBarWidget.cpp"
      #"${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBTicker.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBPanel.cpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBCommandline.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyClicker.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyPointer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyAlpha.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyMouseScroll.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyMouseAxis.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyMouseButton.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyPointer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyKeyboardButton.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/Key.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Log.cpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE.cpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE/SGTechniqueResolverListener.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Player.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run.cpp"
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
      "${MAIN_SOURCE_DIR}/BATB/Run/Notify.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Notify/TBNotify.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/World.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/workers.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Screen.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Time.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Value.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/values.cpp"
      "${MAIN_SOURCE_DIR}/helpers/glfw.cpp"
      "${MAIN_SOURCE_DIR}/Module.cpp"
      "${MAIN_SOURCE_DIR}/File.cpp"
      "${MAIN_SOURCE_DIR}/main.cpp"

      ## nanovg
      ## TODO: make cmake library
      "${CMAKE_SOURCE_DIR}/libs/nanovg/src/nanovg.c"

      ## demo:
      "${MAIN_SOURCE_DIR}/BATB/Demo.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/World.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/LoadWorld.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Course.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/CourseCurve.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration/IterationDemo.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration/IterationDemoForest.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/DemoMapDrawer.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/al.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/gl.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg.cpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/demo.c"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/perf.c"
   )

set(  BATB_INCLUDES
      "${MAIN_SOURCE_DIR}/BATB.hpp"
      "${MAIN_SOURCE_DIR}/BATB/AL.hpp"
      "${MAIN_SOURCE_DIR}/BATB/CourseDrawer.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Course.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/CourseCurve.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration/IterationDemo.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/Iteration/IterationDemoForest.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/KeySet.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/DemoMapDrawer.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/World.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/LoadWorld.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/al.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/gl.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/demo.h"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/perf.h"
      "${MAIN_SOURCE_DIR}/BATB/Demo/libs/nanovg/stb_image_write.h"
      "${MAIN_SOURCE_DIR}/BATB/Event.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Event/EventBase.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Event/EventEater.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Event/EventList.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Aim.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Control.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/DTMovable.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Iteration.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Iteration/IterationForest.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/KeySet.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Map.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/WorldDrawer.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/WorldDrawer/Camera.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/MapDrawer2D.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Sound.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/AimKeysController.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/AimControllable.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Runner.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Trace.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/StepDT.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Terrain.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Terrain/TerrainPageProvider.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/Weather.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/World.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/LoadWorld.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Forest/events.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GL.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/tb_config.h"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/tb_system_batb.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBRendererGL330.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBProgressBarWidget.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBPanel.hpp"
      "${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBCommandline.hpp"
      #"${MAIN_SOURCE_DIR}/BATB/GUI/tb/TBTicker.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/Code.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/Key.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyAlpha.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyKeyboardButton.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyClicker.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyMouseAxis.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyMouseButton.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyMouseScroll.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeyPointer.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/KeySet.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Keys/helpers.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Log.hpp"
      "${MAIN_SOURCE_DIR}/BATB/LogIndent.hpp"
      "${MAIN_SOURCE_DIR}/BATB/ModuleBATB.hpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE.hpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE/SGTechniqueResolverListener.hpp"
      "${MAIN_SOURCE_DIR}/BATB/OGRE/helpers.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Player.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Race.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Race/Prim/Iteration.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Race/RaceClass.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Race/RaceClassEntry.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Race/RacePunch.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Race/RaceWorld.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/TBConsole.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/cmd.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Console/parse.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRun.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunDemo.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunMain.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunMain/TBMain.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunOld.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunWork.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Iteration/IterationRunWork/RunWorkTBWidget.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/KeySet.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Notify.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Notify/NotifyMessage.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/Notify/TBNotify.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/World.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/events.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Run/workers.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Scene.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Screen.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Shape.hpp"
      "${MAIN_SOURCE_DIR}/BATB/ThreadGLContext.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Time.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Value.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/BATB.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/Forest.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/Keys.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/Run.hpp"
      "${MAIN_SOURCE_DIR}/BATB/Value/tb.hpp"
      "${MAIN_SOURCE_DIR}/BATB/glm.hpp"
      "${MAIN_SOURCE_DIR}/Module.hpp"
      "${MAIN_SOURCE_DIR}/debug/gl.hpp"
      "${MAIN_SOURCE_DIR}/definitions.hpp"
      "${MAIN_SOURCE_DIR}/file.hpp"
      "${MAIN_SOURCE_DIR}/game.hpp"
      "${MAIN_SOURCE_DIR}/game/Iteration.hpp"
      "${MAIN_SOURCE_DIR}/game/Iteration/IterationBegin.hpp"
      "${MAIN_SOURCE_DIR}/game/Iteration/IterationFunction.hpp"
      "${MAIN_SOURCE_DIR}/game/Iteration/IterationId.hpp"
      "${MAIN_SOURCE_DIR}/game/IterationStack.hpp"
      "${MAIN_SOURCE_DIR}/game/Output.hpp"
      "${MAIN_SOURCE_DIR}/game/StepDT.hpp"
      "${MAIN_SOURCE_DIR}/game/iterate.hpp"
      "${MAIN_SOURCE_DIR}/helpers/bezier.hpp"
      "${MAIN_SOURCE_DIR}/helpers/follow_value.hpp"
      "${MAIN_SOURCE_DIR}/helpers/timed_value.hpp"
      "${MAIN_SOURCE_DIR}/helpers/helpers_gl.hpp"
      "${MAIN_SOURCE_DIR}/helpers/helpers_tinyxml2.hpp"
      "${MAIN_SOURCE_DIR}/helpers/yaml.hpp"
      "${MAIN_SOURCE_DIR}/helpers/glfw.hpp"
      "${MAIN_SOURCE_DIR}/include.hpp"
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

      # here we should also add header files, but it does not matter if we are not
      # doing any more development of old-batb, which is true (for me :) )
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
    # append headers to sources. this is necessary to recompile changed headers, see
    #   * https://stackoverflow.com/questions/13703647/how-to-properly-add-include-directories-with-cmake#13703725
    #   * https://stackoverflow.com/questions/17653738/recursive-cmake-search-for-header-and-source-files
    #   * https://stackoverflow.com/questions/32411963/why-is-cmake-file-glob-evil#32412044
    list( APPEND BATB_SOURCES ${BATB_INCLUDES} )
    

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
    target_include_directories( open-forest PRIVATE "${MAIN_SOURCE_DIR}/batb/gui/tb" ) # point to _our_ tb_config.h
    if ( APPLE )
        target_compile_definitions( open-forest PUBLIC MACOSX ) # turbobadger needs MACOSX
    endif()

    # yaml-cpp
    list( APPEND BATB_LIBRARIES yaml-cpp )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/yaml-cpp/include" )

    # Ogre Components
    list( APPEND BATB_LIBRARIES OgreMain )
    list( APPEND BATB_LIBRARIES OgreTerrain )
    list( APPEND BATB_LIBRARIES OgrePaging )
    list( APPEND BATB_LIBRARIES OgreHLMS )
    list( APPEND BATB_LIBRARIES OgreMeshLodGenerator )
    list( APPEND BATB_LIBRARIES OgreOverlay )
    list( APPEND BATB_LIBRARIES OgreProperty )
    list( APPEND BATB_LIBRARIES OgreRTShaderSystem )
    list( APPEND BATB_LIBRARIES OgreVolume )

#Ogre.framework

    # FIXME: use Framework folders in build tree
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/OgreMain/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Paging/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Terrain/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/RTShaderSystem/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/MeshLodGenerator/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/HLMS/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Overlay/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Property/include" )
    #target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre/Components/Volume/include" )
    target_include_directories( open-forest PRIVATE "${LIBS_SOURCE_DIR}/ogre" )
    target_include_directories( open-forest PRIVATE "${CMAKE_BINARY_DIR}/include" ) # we need the build generated XXXX
    target_include_directories( open-forest PRIVATE "${LIBS_BINARY_DIR}/ogre/include" ) # we need the build generated YYYY
    # "${LIBS_SOURCE_DIR}/ogre/RenderSystems/GL/include" <- must not be included, it overrides glew.h !!!


    if ( OGRE_BUILD_RENDERSYSTEM_GL )
        list( APPEND BATB_LIBRARIES RenderSystem_GL )
    endif()
    if ( OGRE_BUILD_RENDERSYSTEM_GL3Plus )
        list( APPEND BATB_LIBRARIES RenderSystem_GL3Plus )
    endif()


    # FIXME: clean up
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


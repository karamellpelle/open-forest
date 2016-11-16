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

# open-forest
project( open-forest )

# FIXME: is the -g switch added down to Ogre, even if CMAKE_BUILD_TYPE is 
#        DebWithRelInfo for Ogre??
#        
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
  set( flags_cpp -g -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-deprecated-declarations -Wno-overloaded-virtual )
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



set( MAIN_SOURCE_DIR          "${CMAKE_SOURCE_DIR}/source" )
set( LIBS_SOURCE_DIR          "${CMAKE_SOURCE_DIR}/libs" )



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
set( CMAKE_BUILD_TYPE "None" CACHE STRING "" FORCE )                    # prevent "RelWithDebInfo" 
set( OGRE_BUILD_COMPONENT_OVERLAY             ON CACHE BOOL "" FORCE)   # overlay component
set( OGRE_BUILD_COMPONENT_PAGING              ON CACHE BOOL "" FORCE)   # paging component
set( OGRE_BUILD_COMPONENT_PROPERTY            ON CACHE BOOL "" FORCE)   # property component
set( OGRE_BUILD_COMPONENT_RTSSHADERSYSTEM     ON CACHE BOOL "" FORCE)   # rtsshader componentCMAKE_CURRENT_SOURCE_DIR
set( OGRE_BUILD_COMPONENT_TERRAIN             ON CACHE BOOL "" FORCE)   # terrain component
set( OGRE_BUILD_COMPONENT_VOLUME              ON CACHE BOOL "" FORCE)   # volume component
set( OGRE_BUILD_COMPONENT_BSP                 ON CACHE BOOL "" FORCE)   # bsp component
set( OGRE_BUILD_PLUGIN_CG                     ON CACHE BOOL "" FORCE)   # Cg plugin
set( OGRE_BUILD_PLUGIN_OCTREE                 ON CACHE BOOL "" FORCE)   # octree plugin
set( OGRE_BUILD_PLUGIN_PCZ                    ON CACHE BOOL "" FORCE)   # PCZ plugin
set( OGRE_BUILD_PLUGIN_PFX                    ON CACHE BOOL "" FORCE)   # PFX plugin
set( OGRE_BUILD_RENDERSYSTEM_GL               ON CACHE BOOL "" FORCE)   # GL-rendering
set( OGRE_BUILD_RENDERSYSTEM_GL3Plus          ON CACHE BOOL "" FORCE)   # GL3+-rendering
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

# FIXME!
set( CMAKE_BUILD_TYPE_COPY "${CMAKE_BUILD_TYPE}" )
set( CMAKE_CXX_FLAGS_COPY "${CMAKE_CXX_FLAGS}" )
set( CMAKE_C_FLAGS_COPY "${CMAKE_C_FLAGS}" )
set( CMAKE_BUILD_TYPE "Release" )
#set( CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend" )
#set( CMAKE_CXX_FLAGS "-Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend" )
#set( CMAKE_C_FLAGS "-Wall -Wno-switch -Wno-unused-variable -Wno-unused-function" ) 
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/libs/open-forest-ogre" )
set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_COPY} )
set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_COPY} )
set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS_COPY} )
#


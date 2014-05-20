# for help, see
# http://www.cmake.org/cmake/help/cmake_tutorial.html
# http://www.cmake.org/Wiki/CMake_Useful_Variables
cmake_minimum_required( VERSION 2.6 )
project ( batb )

# set version
set(BATB_VERSION_MAJOR 0)
set(BATB_VERSION_MAJOR 1)

# set compiler flags
# FIXME: into build option
set( CMAKE_CXX_FLAGS "-Wall -Wno-switch -g -pg" )


# set folders
set( MAIN_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/source )
set( BATB_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/source/BATB )
set( ENV_SOURCE_DIR           ${CMAKE_SOURCE_DIR}/source/Env )
set( TEST_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/source/test )
set( LIBS_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs )
set( CUSTOM_CMAKELISTS_DIR    ${CMAKE_SOURCE_DIR}/cmake/libs )
#set( MISC_SOURCE_DIRS   ${CMAKE_SOURCE_DIR}/source/test )


################################################################################
# build library 'glfw'
#
# there seems to be a bug, GLFW_LIBRARIES does not contain libXcursor.
# bug may go away if uncomment these 3 disables below, first time (??)
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   # we do not want to build examples
set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )      # we do not want to build documentation
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )     # we do not want to build tests 
set( GLFW_SOURCE_DIR ${LIBS_SOURCE_DIR}/glfw )

add_subdirectory( ${GLFW_SOURCE_DIR} )

# add GLFW_LIBRARIES, created by GLFW-CMakeLists.txt, to our libraries 
set( BATB_LIBRARIES ${BATB_LIBRARIES} ${GLFW_LIBRARIES} )
include_directories( "${GLFW_SOURCE_DIR}/include" )


################################################################################
# build library 'custom-nanovg'
# this will create
#   custom-nanovg                     : target
#   CUSTOM_NANOVG_INCLUDE_DIRS        : include directory for library
#   CUSTOM_NANOVG_LIBRARIES           : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/nanovg )
include_directories( ${CUSTOM_NANOVG_INCLUDE_DIRS} )
set( BATB_LIBRARIES ${BATB_LIBRARIES} ${CUSTOM_NANOVG_LIBRARIES} )

################################################################################
# build library 'custom-turbobadger'
# this will create
#   custom-turbobadger                : target
#   CUSTOM_TURBOBADGER_INCLUDE_DIRS   : include directories for library
#   CUSTOM_TURBOBADGER_LIBRARIES      : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/turbobadger )
include_directories( ${CUSTOM_TURBOBADGER_INCLUDE_DIRS} )
set( BATB_LIBRARIES ${BATB_LIBRARIES} ${CUSTOM_TURBOBADGER_LIBRARIES} )


# 
################################################################################
# set additional include directories
#
include_directories( "${MAIN_SOURCE_DIR}" )
include_directories( "${LIBS_SOURCE_DIR}/tinyxml2" )
include_directories( "${LIBS_SOURCE_DIR}/glm" )
include_directories( "${BATB_SOURCE_DIR}/tmp" )

# set CMake working directory
set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build )
# ..., but place executables in this folder instead:
#set( EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build )


# options and default settings
option( BATB_BUILD   "Build BATB" ON)
option( BATB_INSTALL "Install BATB" OFF)



################################################################################
# build old sources
#
add_definitions( -DPU_USE_NONE ) # remove GLUT from plib
include_directories( ${MAIN_SOURCE_DIR}/BATB/old/ )
set( OLD_LIBRARIES -lplibpuaux -lplibpu -lplibfnt -lplibsg -lplibul -lGLEW -lGL -lGLU -lglut -lm -lc)

set( OLD_SOURCES 
      ${MAIN_SOURCE_DIR}/BATB/old/old.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/calcalt.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/client.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/common.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/control.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/course.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/database.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgCommon.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgCourse.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgEvent.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgHost1.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgHost2.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgJoin1.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgJoin2.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgMap.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgMenu.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgOptions.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgOrienteer.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgPlayers.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgResult.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgSelect.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgSingle.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgStack.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/dlgTerrain.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/event.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/eventproxy.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/fdManager.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/forest.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/global.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/mainWindow.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/map.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/mapproxy.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/md5.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/mesh.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/message.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/noise.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/orienteer.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/oriproxy.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/point.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/random.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/run.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/server.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/shader.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/terrain.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/terrainproxy.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/texture.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/util.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/windowMessage.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/xmlParser.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/xmountains.cpp
      ${MAIN_SOURCE_DIR}/BATB/old/mainWindow.cpp
)

# define sources
set( BATB_SOURCES
      ${MAIN_SOURCE_DIR}/main.cpp

      ${MAIN_SOURCE_DIR}/Env.cpp
      ${MAIN_SOURCE_DIR}/Env/Screen.cpp

      ${MAIN_SOURCE_DIR}/File.cpp

      ${MAIN_SOURCE_DIR}/BATB.cpp
      ${MAIN_SOURCE_DIR}/BATB/Values.cpp
      ${MAIN_SOURCE_DIR}/BATB/Log.cpp
      ${MAIN_SOURCE_DIR}/BATB/Keys.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/RunKeys.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/RunPrim.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/Prim/IterationRunBegin.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/Prim/IterationRunMain.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/Prim/IterationRunOld.cpp
      ${MAIN_SOURCE_DIR}/BATB/Forest.cpp
      ${MAIN_SOURCE_DIR}/BATB/Forest/ForestKeys.cpp
      ${MAIN_SOURCE_DIR}/BATB/Forest/ForestPrim.cpp
      ${MAIN_SOURCE_DIR}/BATB/Keys/KeyClicker.cpp
      ${MAIN_SOURCE_DIR}/BATB/Keys/KeyPointer.cpp

      # add tmp for now
      ${MAIN_SOURCE_DIR}/BATB/tmp/demo.c
      ${MAIN_SOURCE_DIR}/BATB/tmp/perf.c


      # add old sources for now
      ${OLD_SOURCES}
   )

### BUILD ###
if ( BATB_BUILD )
    # FIXME: into custom
    add_library(tinyxml2 STATIC ${LIBS_SOURCE_DIR}/tinyxml2/tinyxml2.cpp)


    set( BATB_LIBRARIES ${BATB_LIBRARIES} ${OLD_LIBRARIES} -lGLEW )

    # create executable target 'batb'
    add_executable( batb ${BATB_SOURCES} )

    # define target properties
    set_target_properties( batb PROPERTIES
                           OUTPUT_NAME "batb"
                           #CLEAN_DIRECT_OUTPUT 1
                           #VERSION ${})
                         )
   
    # libraries to target
    target_link_libraries( batb glfw tinyxml2 custom-nanovg ${BATB_LIBRARIES} )

endif()



### INSTALL ###
if ( BATB_INSTALL )
    message( "install not implemented..." )
    #install( TARGETS batb DESTINATION bin)
    #install( FILES   "data/" DESTINATION bin)
endif()




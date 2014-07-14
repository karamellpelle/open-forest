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
set( CMAKE_CXX_FLAGS      "-std=c++11 -g -pg -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function -Wno-non-template-friend" )
set( CMAKE_C_FLAGS        "-g -pg -Wall -Wno-switch -Wno-unused-variable -Wno-unused-function" )

# set folders
set( MAIN_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/source )
set( LIBS_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/libs )
set( TEST_SOURCE_DIR          ${CMAKE_SOURCE_DIR}/source/test )
set( CUSTOM_CMAKELISTS_DIR    ${CMAKE_SOURCE_DIR}/cmake/libs )


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


################################################################################
# build library 'custom-nanovg'
# this will create
#   custom-nanovg                     : target
#   CUSTOM_NANOVG_INCLUDE_DIRS        : include directory for library
#   CUSTOM_NANOVG_LIBRARIES           : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/nanovg )
include_directories( ${CUSTOM_NANOVG_INCLUDE_DIRS} )

################################################################################
# build library 'custom-turbobadger'
# this will create
#   custom-turbobadger                : target
#   CUSTOM_TURBOBADGER_INCLUDE_DIRS   : include directories for library
#   CUSTOM_TURBOBADGER_LIBRARIES      : link libs for library
add_subdirectory( ${CUSTOM_CMAKELISTS_DIR}/turbobadger )
include_directories( ${CUSTOM_TURBOBADGER_INCLUDE_DIRS} )


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
      "${MAIN_SOURCE_DIR}/batb/xml/XML.cpp"
      "${MAIN_SOURCE_DIR}/batb/value/Value.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/Keys.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/KeyClicker.cpp"
      "${MAIN_SOURCE_DIR}/batb/keys/KeyPointer.cpp"
      "${MAIN_SOURCE_DIR}/batb/gui/GUI.cpp"
      "${MAIN_SOURCE_DIR}/batb/gui/tb_system.cpp"

      # BATB non-core:
      #"${MAIN_SOURCE_DIR}/batb/forest/Forest.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/Run.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/KeySet.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRun.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRunBegin.cpp"
      "${MAIN_SOURCE_DIR}/batb/run/iteration/IterationRunEnd.cpp"

      # add tmp for now
      "${MAIN_SOURCE_DIR}/batb/tmp/demo.c"
      "${MAIN_SOURCE_DIR}/batb/tmp/perf.c"

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

    # define main
    set( MAIN_LIBRARIES "" )
    set( MAIN_SOURCES "${MAIN_SOURCE_DIR}/main.cpp" )

    # FIXME: into custom
    add_library(tinyxml2 STATIC ${LIBS_SOURCE_DIR}/tinyxml2/tinyxml2.cpp)
    list( APPEND MAIN_LIBRARIES tinyxml2 )
    
    # GLEW
    list( APPEND MAIN_LIBRARIES -lGLEW )

    # GLFW
    include_directories( "${GLFW_SOURCE_DIR}/include" )
    list( APPEND MAIN_LIBRARIES glfw ${GLFW_LIBRARIES} )

    # nanovg
    list( APPEND MAIN_LIBRARIES custom-nanovg ${CUSTOM_NANOVG_LIBRARIES} )

    # OLD
# FIXME: ifdef
    #add_definitions( -DPU_USE_NONE ) # remove GLUT from plib
    #include_directories( "${MAIN_SOURCE_DIR}/batb/old" )
    #set( BATB_OLD_LIBRARIES -lplibpuaux -lplibpu -lplibfnt -lplibsg -lplibul -lGLEW -lGL -lGLU -lglut -lm -lc)
    #list( APPEND MAIN_LIBRARIES ${BATB_OLD_LIBRARIES} )
    #list( APPEND MAIN_SOURCES ${BATB_OLD_SOURCES} )
# FIXME: endif

    # turbobadger
    include_directories( ${MAIN_SOURCE_DIR}/batb/gui )
    list( APPEND MAIN_LIBRARIES custom-turbobadger ${CUSTOM_TURBOBADGER_LIBRARIES} )

    # batb
    list( APPEND MAIN_LIBRARIES ${BATB_LIBRARIES} )
    list( APPEND MAIN_SOURCES ${BATB_SOURCES} ${FILE_SOURCES} ${ENV_SOURCES})


    ###########################################################
    # create executable target 'batb'
    add_executable( batb ${MAIN_SOURCES} ) 

    # define target properties
    set_target_properties( batb PROPERTIES
                           OUTPUT_NAME "batb"
                           #CLEAN_DIRECT_OUTPUT 1
                           #VERSION ${})
                         )
   
    # libraries to target
    target_link_libraries( batb ${MAIN_LIBRARIES} )

endif()



### INSTALL ###
if ( BATB_INSTALL )
    message( "install not implemented..." )
    #install( TARGETS batb DESTINATION bin)
    #install( FILES   "data/" DESTINATION bin)
endif()




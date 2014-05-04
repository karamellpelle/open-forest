# for help, see
# http://www.cmake.org/cmake/help/cmake_tutorial.html
# http://www.cmake.org/Wiki/CMake_Useful_Variables
cmake_minimum_required( VERSION 2.6 )
project ( batb )

# set version
set(BATB_VERSION_MAJOR 0)
set(BATB_VERSION_MAJOR 1)

# set folders
set( MAIN_SOURCE_DIR   ${CMAKE_SOURCE_DIR}/source )
set( BATB_SOURCE_DIR   ${CMAKE_SOURCE_DIR}/source/BATB )
set( ENV_SOURCE_DIR    ${CMAKE_SOURCE_DIR}/source/Env )
set( TEST_SOURCE_DIR   ${CMAKE_SOURCE_DIR}/source/test )
set( LIBS_SOURCE_DIR   ${CMAKE_SOURCE_DIR}/libs )
#set( MISC_SOURCE_DIRS   ${CMAKE_SOURCE_DIR}/source/test )


# set compiler flags
# FIXME: into build option
set( CMAKE_CXX_FLAGS "-Wall -g -pg" )

# set GLFW 
# there is currently a bug, GLFW_LIBRARIES does not contain libXcursor, if we disable these below.
# FIXME: enable later...
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)   # we do not want to build examples
set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )      # we do not want to build documentation
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )     # we do not want to build tests 
set( GLFW_SOURCE_DIR ${LIBS_SOURCE_DIR}/glfw )
add_subdirectory( ${GLFW_SOURCE_DIR} )
include_directories( "${GLFW_SOURCE_DIR}/include" )
set( BATB_LIBRARIES ${GLFW_LIBRARIES} )

#endif GLFW...


# set CMake working directory
set( CMAKE_CURRENT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build )
# ..., but place executables in this folder instead:
#set( EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build )


# options and default settings
option( BATB_BUILD   "Build BATB" ON)
option( BATB_INSTALL "Install BATB" OFF)


# define sources
include_directories( ${MAIN_SOURCE_DIR} )
include_directories( ${LIBS_SOURCE_DIR}/tinyxml2 )
include_directories( ${LIBS_SOURCE_DIR}/glm )
set( BATB_SOURCES
      ${MAIN_SOURCE_DIR}/main.cpp
      ${MAIN_SOURCE_DIR}/Env.cpp
      ${MAIN_SOURCE_DIR}/Env/Screen.cpp
      ${MAIN_SOURCE_DIR}/File.cpp
      ${MAIN_SOURCE_DIR}/BATB.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/RunData.cpp
      ${MAIN_SOURCE_DIR}/BATB/Run/Prim/IterationRunBegin.cpp
      ${MAIN_SOURCE_DIR}/BATB/Forest/ForestData.cpp
      ${MAIN_SOURCE_DIR}/BATB/Log.cpp
      ${MAIN_SOURCE_DIR}/BATB/Values.cpp
      ${MAIN_SOURCE_DIR}/BATB/Keys.cpp
   )

### BUILD ###
if ( BATB_BUILD )
    # create executable target 'batb'
    add_executable( batb ${BATB_SOURCES})
    # define target properties
    set_target_properties( batb PROPERTIES
                           OUTPUT_NAME "batb"
                           #CLEAN_DIRECT_OUTPUT 1
                           #VERSION ${})
                         )
    # libraries to target
    target_link_libraries( batb glfw ${BATB_LIBRARIES} )
endif()



### INSTALL ###
if ( BATB_INSTALL )
    message( "install not implemented..." )
    #install( TARGETS batb DESTINATION bin)
    #install( FILES   "data/" DESTINATION bin)
endif()

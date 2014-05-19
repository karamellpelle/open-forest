project ( custom-nanovg C )
cmake_minimum_required( VERSION 2.6 )


set( CUSTOM_NANOVG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/nanovg )
set( CUSTOM_NANOVG_INCLUDE_DIRS ${CUSTOM_NANOVG_SOURCE_DIR}/src )
#FIXME: set <project-name>_SOURCE_DIR <project-name>_BIN_DIR from these??
#       (http://www.cmake.org/cmake/help/v2.8.12/cmake.html#command:project)

#message( "CUSTOM_NANOVG_SOURCE_DIR ${CUSTOM_NANOVG_SOURCE_DIR}" )
#message( "CUSTOM_NANOVG_INCLUDE_DIRS ${CUSTOM_NANOVG_INCLUDE_DIRS}" )


set(  CUSTOM_NANOVG_SOURCES 
      "${CUSTOM_NANOVG_SOURCE_DIR}/src/nanovg.c"
      "${CUSTOM_NANOVG_SOURCE_DIR}/src/stb_image.c"
) 

# DEBUG/NDEBUG
#add_definitions( -DNDEBUG ) 
include_directories( $CUSTOM_NANOVG_INCLUDE_DIRS )

# create static library 'nanovg-custom'
add_library(custom-nanovg STATIC ${CUSTOM_NANOVG_SOURCES})


#--------------------------------------------------------------------
# Export CUSTOM_NANOVG_XXX
#--------------------------------------------------------------------
set(CUSTOM_NANOVG_INCLUDE_DIRS ${CUSTOM_NANOVG_INCLUDE_DIRS} CACHE STRING "include dirs for custom-nanovg")

project ( custom-tinyxml2 CXX )
cmake_minimum_required( VERSION 2.6 )


set( CUSTOM_TINYXML2_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/tinyxml2 )
set( CUSTOM_TINYXML2_INCLUDE_DIRS ${CUSTOM_TINYXML2_SOURCE_DIR} )
#FIXME: set <project-name>_SOURCE_DIR <project-name>_BIN_DIR from these??
#       (http://www.cmake.org/cmake/help/v2.8.12/cmake.html#command:project)

#message( "CUSTOM_TINYXML2_SOURCE_DIR ${CUSTOM_TINYXML2_SOURCE_DIR}" )
#message( "CUSTOM_TINYXML2_INCLUDE_DIRS ${CUSTOM_TINYXML2_INCLUDE_DIRS}" )


set(  CUSTOM_TINYXML2_SOURCES 
      "${CUSTOM_TINYXML2_SOURCE_DIR}/tinyxml2.cpp"
) 

# DEBUG/NDEBUG
#add_definitions( -DNDEBUG ) 
include_directories( ${CUSTOM_TINYXML2_INCLUDE_DIRS} )

# create static library 'tinyxml2-custom'
add_library(custom-tinyxml2 STATIC ${CUSTOM_TINYXML2_SOURCES})


#--------------------------------------------------------------------
# Export CUSTOM_TINYXML2_XXX
#--------------------------------------------------------------------
set(CUSTOM_TINYXML2_INCLUDE_DIRS ${CUSTOM_TINYXML2_INCLUDE_DIRS} PARENT_SCOPE)
set(CUSTOM_TINYXML2_LIBRARIES    ""                            PARENT_SCOPE)

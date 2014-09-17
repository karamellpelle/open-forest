cmake_minimum_required( VERSION 3.0 )

project ( my-tinyxml2 CXX )


set( MY-TINYXML2_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/tinyxml2 )
set( MY-TINYXML2_INCLUDE_DIRS ${MY-TINYXML2_SOURCE_DIR} )

set(  MY-TINYXML2_SOURCES 
      "${MY-TINYXML2_SOURCE_DIR}/tinyxml2.cpp"
   ) 

# create static library 'my-tinyxml2'
add_library(my-tinyxml2 STATIC ${MY-TINYXML2_SOURCES} )

target_include_directories( my-tinyxml2 PUBLIC ${MY-TINYXML2_INCLUDE_DIRS} )



cmake_minimum_required( VERSION 3.0 )
project ( my-nanovg C )


set( MY-NANOVG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/nanovg )
set( MY-NANOVG_INCLUDE_DIRS ${MY-NANOVG_SOURCE_DIR}/src )


set(  MY-NANOVG_SOURCES 
      "${MY-NANOVG_SOURCE_DIR}/src/nanovg.c"
      "${MY-NANOVG_SOURCE_DIR}/src/stb_image.c"
   ) 

# create static library 'my-nanovg'
add_library( my-nanovg STATIC ${MY-NANOVG_SOURCES})
target_include_directories( my-nanovg PUBLIC ${MY-NANOVG_INCLUDE_DIRS} )


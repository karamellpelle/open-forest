cmake_minimum_required( VERSION 3.0 )

################################################################################
# see CMakeLists.txt files in turbobadger folder!!

project ( my-turbobadger )

# TODO: not just linux... but gives compile error otherwise in tb_select.cpp
#add_compile_options( -DLINUX )

set( MY-TURBOBADGER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/turbobadger )
list( APPEND MY-TURBOBADGER_INCLUDE_DIRS ${MY-TURBOBADGER_SOURCE_DIR}/src )
list( APPEND MY-TURBOBADGER_INCLUDE_DIRS ${MY-TURBOBADGER_SOURCE_DIR}/src/tb )

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -Wall -Wextra -Wno-unused-parameter") # -fno-rtti removed!


set( MY-TURBOBADGER_SOURCES "")
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb            MY-TURBOBADGER_SOURCES)
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb/image      MY-TURBOBADGER_SOURCES)
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb/animation  MY-TURBOBADGER_SOURCES)
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb/parser     MY-TURBOBADGER_SOURCES)
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb/renderers  MY-TURBOBADGER_SOURCES)
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tests      MY-TURBOBADGER_SOURCES)
aux_source_directory(${MY-TURBOBADGER_SOURCE_DIR}/src/tb/utf8       MY-TURBOBADGER_SOURCES)


# 
include_directories(  "${MAIN_SOURCE_DIR}/BATB/GUI/tb" ) # make sure we use tb_config.h 
include_directories("${PROJECT_SOURCE_DIR}/src/tb")

#find_package( OpenGL )
#include_directories( ${OPENGL_INCLUDE_DIR} )  

if(APPLE)
 add_definitions(-DMACOSX)
endif(APPLE)


# create static library 'my-turbobadger'
add_library( my-turbobadger STATIC ${MY-TURBOBADGER_SOURCES} )


# add our own tb_config.h: FIXME: verify that this is seached before the tb-folder
target_include_directories( my-turbobadger PRIVATE "${MAIN_SOURCE_DIR}/BATB/GUI/tb" ) 
#target_include_directories( my-turbobadger PRIVATE "${MAIN_SOURCE_DIR}" )
target_include_directories( my-turbobadger PUBLIC ${MY-TURBOBADGER_INCLUDE_DIRS} )  

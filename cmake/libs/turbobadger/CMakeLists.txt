project ( custom-turbobadger C )
cmake_minimum_required( VERSION 2.6 )

# set flags, based on turbobadger Makefile
# we see that turbobadger wants nullptr and some other stuff
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX -Dnullptr=0 --std=c++0x" ) # FIXME: not just linux... but gives compile error otherwise in tb_select.cpp

set( CUSTOM_TURBOBADGER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/turbobadger )
list( APPEND CUSTOM_TURBOBADGER_INCLUDE_DIRS ${CUSTOM_TURBOBADGER_SOURCE_DIR}/src )
list( APPEND CUSTOM_TURBOBADGER_INCLUDE_DIRS ${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb )


#FIXME: set <project-name>_SOURCE_DIR <project-name>_BIN_DIR from these??
#       (http://www.cmake.org/cmake/help/v2.8.12/cmake.html#command:project)

#message( "CUSTOM_TURBOBADGER_SOURCE_DIR ${CUSTOM_TURBOBADGER_SOURCE_DIR}" )
#message( "CUSTOM_TURBOBADGER_INCLUDE_DIRS ${CUSTOM_TURBOBADGER_INCLUDE_DIRS}" )


set(  CUSTOM_TURBOBADGER_SOURCES 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_layout.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_scroller.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_scroll_container.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_skin.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_skin_util.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_bitmap_fragment.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_inline_select.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_select.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_select_item.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_editfield.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_system_linux.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_clipboard_glfw.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_tab_container.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets_common.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_toggle_container.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets_reader.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_widget_value.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_widget_skin_condition_context.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_window.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_debug.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_message_window.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_popup_window.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_menu_window.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_addon.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_core.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_dimension.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_id.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_str.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_color.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_tempbuffer.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_hash.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_hashtable.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_value.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_linklist.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_list.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_style_edit.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_style_edit_content.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_language.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_msg.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_object.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets_listener.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_renderer.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_font_renderer.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_font_renderer_tbbf.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_geometry.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_node_tree.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_node_ref_tree.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/tb_image_loader_stb.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/parser/tb_parser.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/utf8/utf8.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/animation/tb_animation.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/animation/tb_widget_animation.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/addons/tbimage/tb_image_manager.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/addons/tbimage/tb_image_widget.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/renderers/tb_renderer_batcher.cpp" 
      "${CUSTOM_TURBOBADGER_SOURCE_DIR}/src/tb/renderers/tb_renderer_gl.cpp" 

) 

# include directories.
# add MAIN_SOURCE_DIR (inherited) before, in order to find our tb_config.h
include_directories( ${MAIN_SOURCE_DIR} ${CUSTOM_TURBOBADGER_INCLUDE_DIRS} )  

# create static library 'turbobadger-custom'
add_library(custom-turbobadger STATIC ${CUSTOM_TURBOBADGER_SOURCES})


#--------------------------------------------------------------------
# Export CUSTOM_TURBOBADGER_XXX
#--------------------------------------------------------------------
set(CUSTOM_TURBOBADGER_INCLUDE_DIRS ${CUSTOM_TURBOBADGER_INCLUDE_DIRS}  PARENT_SCOPE)
set(CUSTOM_TURBOBADGER_LIBRARIES    ""                                  PARENT_SCOPE)


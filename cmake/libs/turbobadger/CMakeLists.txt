cmake_minimum_required( VERSION 3.0 )

project ( my-turbobadger CXX )

# set flags, based on turbobadger Makefile
# we see that turbobadger wants nullptr and some other stuff
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x -DLINUX" ) # FIXME: not just linux... but gives compile error otherwise in tb_select.cpp

set( MY-TURBOBADGER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/turbobadger )
list( APPEND MY-TURBOBADGER_INCLUDE_DIRS ${MY-TURBOBADGER_SOURCE_DIR}/src )
list( APPEND MY-TURBOBADGER_INCLUDE_DIRS ${MY-TURBOBADGER_SOURCE_DIR}/src/tb )


set(  MY-TURBOBADGER_SOURCES 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_layout.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_scroller.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_scroll_container.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_skin.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_skin_util.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_bitmap_fragment.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_inline_select.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_select.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_select_item.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_editfield.cpp" 

      # implemenation done by BATB
      #"${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_system_linux.cpp"

      # no clipboard support
      #"${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_clipboard_glfw.cpp" 

      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_tab_container.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets_common.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_toggle_container.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets_reader.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_widget_value.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_widget_skin_condition_context.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_window.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_debug.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_message_window.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_popup_window.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_menu_window.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_addon.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_core.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_dimension.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_id.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_str.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_color.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_tempbuffer.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_hash.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_hashtable.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_value.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_linklist.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_list.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_style_edit.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_style_edit_content.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_language.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_msg.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_object.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_widgets_listener.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_renderer.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_font_renderer.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_font_renderer_tbbf.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_geometry.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_node_tree.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_node_ref_tree.cpp" 

      # we provide our own
      #"${MY-TURBOBADGER_SOURCE_DIR}/src/tb/tb_image_loader_stb.cpp" 
      "${CMAKE_CURRENT_SOURCE_DIR}/src/tb/tb_image_loader_stb.cpp" 

      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/parser/tb_parser.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/utf8/utf8.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/animation/tb_animation.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/animation/tb_widget_animation.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/addons/tbimage/tb_image_manager.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/addons/tbimage/tb_image_widget.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/renderers/tb_renderer_batcher.cpp" 
      "${MY-TURBOBADGER_SOURCE_DIR}/src/tb/renderers/tb_renderer_gl.cpp" 

) 


# create static library 'turbobadger-my'
add_library( my-turbobadger STATIC ${MY-TURBOBADGER_SOURCES} )

target_include_directories( my-turbobadger PRIVATE ${MAIN_SOURCE_DIR} )
target_include_directories( my-turbobadger PUBLIC ${MY-TURBOBADGER_INCLUDE_DIRS} )  


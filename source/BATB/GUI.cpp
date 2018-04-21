//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "BATB/GUI.hpp"
#include "BATB/GUI/tb/tb_system_batb.hpp"
#include "BATB/Scene.hpp"
#include "BATB/Keys.hpp"
#include "BATB/GL.hpp"





namespace batb
{

namespace gui
{


////////////////////////////////////////////////////////////////////////////////
//  

void GUI::begin(const std::string& path)
{
    batb->log << "batb->gui->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // TODO: use 'yaml' for configuration 
        // TODO: check for errors

        ////////////////////////////////////////////////////////////////////////////////
        // this is the actual init process in turbobadger demo:
        // * AppBackendGLFW()
        // * AppBackendGLFW::Init()
        // * DemoApplication::Init()
        // * (main loop)
        // * DemoApplication::Shutdown()
        // * some more

        ////////////////////////////////////////////////////////////////////////////////
        // AppBackendGLFW::Init()
          
        tb_renderer_ =  new tb::TBRendererGL();
        tb::tb_core_init( tb_renderer_ );
        batb->log << "TBRenderer created" << std::endl;



        ////////////////////////////////////////////////////////////////////////////////
        // OnBackendAttached()
        // Load language file
        tb::g_tb_lng->Load( file::static_data( "BATB/GUI/turbobadger_demo/resources/language/lng_en.tb.txt" ).c_str() );
        batb->log << "language loaded (en)" << std::endl;

        // Load the default skin, and override skin that contains the graphics specific to the demo.
        //tb::g_tb_skin->Load( file::static_data( "batb/gui/turbobadger_demo/resources/default_skin/skin.tb.txt" ).c_str() ,  file::static_data( "batb/gui/turbobadger_demo/turbobadger_demo/Demo/demo01/resources/skin.tb.txt" ).c_str() ); // skin from demo
        tb::g_tb_skin->Load( file::static_data( "BATB/GUI/skin/openforest/skin.tb.txt" ).c_str() ,  file::static_data( "BATB/GUI/skin/openforest/override.tb.txt" ).c_str() );  // custom skin
        batb->log << "skin loaded (overrided)" << std::endl;

        // Register font renderers.
        // these are buildt by turbobadger iff defined, and
        // the corresponding functions, used below, are 
        // declared in batb/gui/tb_system.hpp 
#ifdef TB_FONT_RENDERER_TBBF
        register_tbbf_font_renderer();
        batb->log << "TBBFRenderer registered" << std::endl;
#endif
#ifdef TB_FONT_RENDERER_STB
        register_stb_font_renderer();
        batb->log << "STBFontRenderer registered" << std::endl;
#endif
#ifdef TB_FONT_RENDERER_FREETYPE
        register_freetype_font_renderer();
        batb->log << "FreetypeFontRenderer registered" << std::endl;
#endif

        // Add fonts we can use to the font manager.
#if defined(TB_FONT_RENDERER_STB) || defined(TB_FONT_RENDERER_FREETYPE)
        tb::g_font_manager->AddFontInfo( file::static_data( "BATB/GUI/turbobadger_demo/resources/vera.ttf" ).c_str(), "Vera");
        batb->log << "font added: vera.ttf" << std::endl;
#endif
#ifdef TB_FONT_RENDERER_TBBF
        tb::g_font_manager->AddFontInfo( file::static_data( "BATB/GUI/turbobadger_demo/resources/default_font/segoe_white_with_shadow.tb.txt" ).c_str(), "Segoe");
        batb->log << "font added: segoe_white_with_shadow.tb.txt" << std::endl;
        tb::g_font_manager->AddFontInfo( file::static_data( "BATB/GUI/turbobadger_demo/Demo/fonts/neon.tb.txt" ).c_str(), "Neon");
        batb->log << "font added: neon.tb.txt" << std::endl;
        tb::g_font_manager->AddFontInfo( file::static_data( "BATB/GUI/turbobadger_demo/Demo/fonts/orangutang.tb.txt" ).c_str(), "Orangutang");
        batb->log << "font added: orangutang.tb.txt" << std::endl;
        tb::g_font_manager->AddFontInfo( file::static_data( "BATB/GUI/turbobadger_demo/Demo/fonts/orange.tb.txt" ).c_str(), "Orange");
        batb->log << "font added: orange.tb.txt" << std::endl;
#endif

        // Set the default font description for widgets to one of the fonts we just added
        tb::TBFontDescription fd;
#ifdef TB_FONT_RENDERER_TBBF
        fd.SetID(TBIDC("Segoe"));
#else
        fd.SetID(TBIDC("Vera"));
#endif
        fd.SetSize( tb::g_tb_skin->GetDimensionConverter()->DpToPx(14));
        tb::g_font_manager->SetDefaultFontDescription(fd);

        // Create the font now.
        tb::TBFontFace *font = tb::g_font_manager->CreateFontFace(tb::g_font_manager->GetDefaultFontDescription());
        // Render some glyphs in one go now since we know we are going to use them. It would work fine
        // without this since glyphs are rendered when needed, but with some extra updating of the glyph bitmap.
        if (font)
        {
            font->RenderGlyphs(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~•·åäöÅÄÖ");
            batb->log << "TBFontFace created and rendered" << std::endl;
        }
        // Give the root widget a background skin
        //m_root.SetSkinBg(TBIDC("background"));



        ////////////////////////////////////////////////////////////////////////////////
        // DemoApplication::Init()
        ////////////////////////////////////////////////////////////////////////////////
        // App::Init()
        tb::TBWidgetsAnimationManager::Init();
        { tb::TBAnimationBlocker anim_blocker; }
        batb->log << "TBWidgetsAnimationManager initialized" << std::endl;

        // (demo is now set up)
        ////////////////////////////////////////////////////////////////////////////////
        
        // set name of root widget
        root.SetText( "GUI::root" );


    }

    init( true );
}


void GUI::end()
{
    batb->log << "batb->gui->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        save();

        // TODO?
        //root.DeleteAllChildren();

        ////////////////////////////////////////////////////////////////////////////////
        // App::Shutdown()
        tb::TBWidgetsAnimationManager::Shutdown();
        batb->log << "TBWidgetsAnimationManager shut down" << std::endl;


        ////////////////////////////////////////////////////////////////////////////////
        // AppBackendGLFW::Shutdown()
        tb::tb_core_shutdown();
        batb->log << "tb_core shut down" << std::endl;

        delete tb_renderer_;
        tb_renderer_ = nullptr;

        wth_ = 0;
        hth_ = 0;
    }
    
    init( false );
}



// render to Scene
void GUI::output(const Scene& scene)
{
debug::gl::DebugGroup _dbg(DEBUG_FUNCTION_NAME);

    // reset gl-state after turbobadger, for  this frame
    batb->gl->turbobadgerBegin();

    // TODO: bind FBO, ..., from 'scene'
    //       see: tb_renderer_gl.cpp, TBRendererGL::BeginPaint

    // we need to set modelview for TB rendering 
    // TODO: use 'scene'
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity(); 


    ////////////////////////////////////////////////////////////////////////////////
    // App::RenderFrame
    wth_ = scene.wth; hth_ = scene.hth;
    tb::g_renderer->BeginPaint( wth_, hth_ );


    // paint our GUI tree
    root.InvokePaint( tb::TBWidget::PaintProps() );

    tb::g_renderer->EndPaint();

    // If animations are running, reinvalidate immediately
    if ( tb::TBAnimationManager::HasAnimationsRunning() )
            root.Invalidate();

    // reset gl-state after turbobadger, for  this frame
    batb->gl->turbobadgerEnd();

}

void GUI::step(tick_t tick)
{
debug::gl::DebugGroup _dbg(DEBUG_FUNCTION_NAME);

    ////////////////////////////////////////////////////////////////////////////////
    // void App::OnResized(int width, int height)

    // set size of GUI, using our screen
    root.SetRect( tb::TBRect(0, 0, wth_, hth_) );


    /////////////////////////////////////////////////////////////////////////////////////////
    // static void timer_callback()

    // set time for our TBSystem
    tbsystem_ms( tick * 1000.0 );

    // update messages for TB, that is, send TBMessage's from
    // the global message queue to the registered TBMessageHandler
    // for each message
    tb::TBMessageHandler::ProcessMessages(); 


    ////////////////////////////////////////////////////////////////////////////////
    // void App::Process()

    // update animations
    tb::TBAnimationManager::Update();

    // TB stuff
    root.InvokeProcessStates();
    root.InvokeProcess();
    
}

void GUI::bind(keys::Keys* keys)
{
    // binding keys to this GUI object's root widget
    callback_widget = &root;
    
    keys->cursorposCalling(   glfw_callback_cursor_pos );
    keys->mousebuttonCalling( glfw_callback_mouse_button );
    keys->scrollCalling(      glfw_callback_scroll );
    keys->keyCalling(         glfw_callback_key );
    keys->charCalling(        glfw_callback_char );

    // set cursor mode to non-free (pointer type)
    keys->setCursorFree( false );
}


void GUI::addWidget(tb::TBWidget* w)
{
    if ( tb_top_ )
    {
        root.AddChildRelative( w, tb::WIDGET_Z_REL_BEFORE, tb_top_ );
    }
    else
    {
        root.AddChild( w );
    }
}

void GUI::addWidgetTop(tb::TBWidget* w)
{
    root.AddChild( w );
    tb_top_ = w;
}


void GUI::removeWidget(tb::TBWidget* w)
{
    if ( w == tb_top_ ) tb_top_ = nullptr;

    root.RemoveChild( w );
}

void GUI::lockKeys(bool lock)
{
    batb->keys->keyEnable( !lock );
    // (cursor not touched)
}


// the receving widget for keys callbacks (static)
tb::TBWidget* GUI::callback_widget = nullptr;

////////////////////////////////////////////////////////////////////////////////
//  callback helpers
//

static int mouse_x = 0;
static int mouse_y = 0;
static bool key_alt = false;
static bool key_ctrl = false;
static bool key_shift = false;
static bool key_super = false;


// @return Return the upper case of a ascii charcter. Only for shortcut handling.
static inline int toupr_ascii(int ascii)
{
        if (ascii >= 'a' && ascii <= 'z')
                return ascii + 'A' - 'a';
        return ascii;
}


static inline tb::MODIFIER_KEYS GetModifierKeys()
{
        tb::MODIFIER_KEYS code = tb::TB_MODIFIER_NONE;
        if (key_alt)    code |= tb::TB_ALT;
        if (key_ctrl)   code |= tb::TB_CTRL;
        if (key_shift)  code |= tb::TB_SHIFT;
        if (key_super)  code |= tb::TB_SUPER;
        return code;
}

static inline tb::MODIFIER_KEYS GetModifierKeys(int modifier)
{
        tb::MODIFIER_KEYS code = tb::TB_MODIFIER_NONE;
        if (modifier & GLFW_MOD_ALT)            code |= tb::TB_ALT;
        if (modifier & GLFW_MOD_CONTROL)        code |= tb::TB_CTRL;
        if (modifier & GLFW_MOD_SHIFT)          code |= tb::TB_SHIFT;
        if (modifier & GLFW_MOD_SUPER)          code |= tb::TB_SUPER;
        return code;
}

static bool InvokeShortcut(int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down)
{
        if (!tb::TBWidget::focused_widget || !down)
                return false;
#ifdef MACOSX
        bool shortcut_key = (modifierkeys & tb::TB_SUPER) ? true : false;
#else
        bool shortcut_key = (modifierkeys & tb::TB_CTRL) ? true : false;
#endif
        bool reverse_key = (modifierkeys & tb::TB_SHIFT) ? true : false;
        tb::TBID id;
        if (toupr_ascii(key) == 'X' && shortcut_key)
                id = TBIDC("cut");
        else if ((toupr_ascii(key) == 'C' || special_key == tb::TB_KEY_INSERT) && shortcut_key)
                id = TBIDC("copy");
        else if (((toupr_ascii(key) == 'V' && shortcut_key) ||
                        (special_key == tb::TB_KEY_INSERT && reverse_key)))
                id = TBIDC("paste");
        else if (toupr_ascii(key) == 'A' && shortcut_key)
                id = TBIDC("selectall");
        else if ((toupr_ascii(key) == 'Z' && shortcut_key) ||
                        (toupr_ascii(key) == 'Y' && shortcut_key))
        {
                bool undo = toupr_ascii(key) == 'Z';
                if (reverse_key)
                        undo = !undo;
                id = undo ? TBIDC("undo") : TBIDC("redo");
        }
        else
                return false;

        tb::TBWidgetEvent ev(tb::EVENT_TYPE_SHORTCUT);
        ev.modifierkeys = modifierkeys;
        ev.ref_id = id;
        return tb::TBWidget::focused_widget->InvokeEvent(ev);
}

static inline bool InvokeKey(tb::TBWidget* widget, GLFWwindow *window, unsigned int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down)
{
        if (InvokeShortcut(key, tb::TB_KEY_UNDEFINED, modifierkeys, down))
                return true;

        if ( widget )
            widget->InvokeKey( key, special_key, modifierkeys, down );

        return true;
}


////////////////////////////////////////////////////////////////////////////////
//  GLFW callbacks
//  

void GUI::glfw_callback_char(GLFWwindow *window, unsigned int character)
{
        // glfw on osx seems to send us characters from the private
        // use block when using f.ex arrow keys on osx.
        if (character >= 0xE000 && character <= 0xF8FF)
                return;

        InvokeKey( callback_widget, window, character, tb::TB_KEY_UNDEFINED, GetModifierKeys(), true);    // down
        InvokeKey( callback_widget, window, character, tb::TB_KEY_UNDEFINED, GetModifierKeys(), false);   // up
}

void GUI::glfw_callback_key(GLFWwindow *window, int key, int scancode, int action, int glfwmod)
{
        tb::MODIFIER_KEYS modifier = GetModifierKeys(glfwmod);
        bool down = (action == GLFW_PRESS || action == GLFW_REPEAT);

        // assert callback widget
        if ( callback_widget == nullptr ) return;

        switch (key)
        {
        case GLFW_KEY_F1:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F1, modifier, down); break;
        case GLFW_KEY_F2:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F2, modifier, down); break;
        case GLFW_KEY_F3:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F3, modifier, down); break;
        case GLFW_KEY_F4:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F4, modifier, down); break;
        case GLFW_KEY_F5:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F5, modifier, down); break;
        case GLFW_KEY_F6:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F6, modifier, down); break;
        case GLFW_KEY_F7:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F7, modifier, down); break;
        case GLFW_KEY_F8:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F8, modifier, down); break;
        case GLFW_KEY_F9:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_F9, modifier, down); break;
        case GLFW_KEY_F10:                      InvokeKey( callback_widget, window, 0, tb::TB_KEY_F10, modifier, down); break;
        case GLFW_KEY_F11:                      InvokeKey( callback_widget, window, 0, tb::TB_KEY_F11, modifier, down); break;
        case GLFW_KEY_F12:                      InvokeKey( callback_widget, window, 0, tb::TB_KEY_F12, modifier, down); break;
        case GLFW_KEY_LEFT:                     InvokeKey( callback_widget, window, 0, tb::TB_KEY_LEFT, modifier, down); break;
        case GLFW_KEY_UP:                       InvokeKey( callback_widget, window, 0, tb::TB_KEY_UP, modifier, down); break;
        case GLFW_KEY_RIGHT:                    InvokeKey( callback_widget, window, 0, tb::TB_KEY_RIGHT, modifier, down); break;
        case GLFW_KEY_DOWN:                     InvokeKey( callback_widget, window, 0, tb::TB_KEY_DOWN, modifier, down); break;
        case GLFW_KEY_PAGE_UP:                  InvokeKey( callback_widget, window, 0, tb::TB_KEY_PAGE_UP, modifier, down); break;
        case GLFW_KEY_PAGE_DOWN:                InvokeKey( callback_widget, window, 0, tb::TB_KEY_PAGE_DOWN, modifier, down); break;
        case GLFW_KEY_HOME:                     InvokeKey( callback_widget, window, 0, tb::TB_KEY_HOME, modifier, down); break;
        case GLFW_KEY_END:                      InvokeKey( callback_widget, window, 0, tb::TB_KEY_END, modifier, down); break;
        case GLFW_KEY_INSERT:                   InvokeKey( callback_widget, window, 0, tb::TB_KEY_INSERT, modifier, down); break;
        //case GLFW_KEY_TAB:                      InvokeKey( callback_widget, window, 0, tb::TB_KEY_TAB, modifier, down); break; // remove for now; bug in turbobadger when no widget handles focus
        case GLFW_KEY_DELETE:                   InvokeKey( callback_widget, window, 0, tb::TB_KEY_DELETE, modifier, down); break;
        case GLFW_KEY_BACKSPACE:                InvokeKey( callback_widget, window, 0, tb::TB_KEY_BACKSPACE, modifier, down); break;
        case GLFW_KEY_ENTER:                    InvokeKey( callback_widget, window, 0, tb::TB_KEY_ENTER, modifier, down); break;
        case GLFW_KEY_ESCAPE:                   InvokeKey( callback_widget, window, 0, tb::TB_KEY_ESC, modifier, down); break;
        case GLFW_KEY_MENU:
                if (tb::TBWidget::focused_widget && !down)
                {
                        tb::TBWidgetEvent ev(tb::EVENT_TYPE_CONTEXT_MENU);
                        ev.modifierkeys = modifier;
                        tb::TBWidget::focused_widget->InvokeEvent(ev);
                }
                break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
                key_shift = down;
                break;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
                key_ctrl = down;
                break;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
                key_alt = down;
                break;
        case GLFW_KEY_LEFT_SUPER:
        case GLFW_KEY_RIGHT_SUPER:
                key_super = down;
                break;
/* did not work since TBEditField eat up KEY_DOWN event :(
        // a hack to prevent space as a button for ClickByKey (i.e. only enter send an
        // EVENT_TYPE_CLICK if focused widget has ClickByKey enabled, see implemetation
        // of TBWidget::InvokeKey).
        // this goes before 'default' below (space == 32)
        case GLFW_KEY_SPACE:
                //if (key_ctrl && !key_alt ) // a check as below
                        InvokeKey( callback_widget, window, key, tb::TB_KEY_UNDEFINED, 
                                   (tb::MODIFIER_KEYS)(0xffff0000 | modifier), down); // modifier non-zero, bitfields preserved
                break;
*/
        default:
                // glfw calls key_callback instead of char_callback
                // when pressing a character while ctrl is also pressed.
                if (key_ctrl && !key_alt && key >= 32 && key <= 255)
                        InvokeKey( callback_widget, window, key, tb::TB_KEY_UNDEFINED, modifier, down);
                break;
        }
}

void GUI::glfw_callback_mouse_button(GLFWwindow *window, int button, int action, int glfwmod)
{
        tb::MODIFIER_KEYS modifier = GetModifierKeys(glfwmod);
        int x = mouse_x;
        int y = mouse_y;
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
                if (action == GLFW_PRESS)
                {
                        // This is a quick fix with n-click support :)
                        //static double last_time = 0;
                        static int last_x = 0;
                        static int last_y = 0;
                        static int counter = 1;

                        //double time = tb::TBSystem::GetTimeMS();
                        static tick_t last_time = 0;
                        tick_t time = glfwGetTime();

                        if (time < last_time + 600 && last_x == x && last_y == y)
                                counter++;
                        else
                                counter = 1;
                        last_x = x;
                        last_y = y;
                        last_time = time;

                        if ( callback_widget )
                            callback_widget->InvokePointerDown(x, y, counter, modifier, false );
                }
                else
                        if ( callback_widget )
                            callback_widget->InvokePointerUp(x, y, modifier,  false );
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
                if ( callback_widget )
                    callback_widget->InvokePointerMove(x, y, modifier, false );

                if (tb::TBWidget::hovered_widget)
                {
                        tb::TBWidget::hovered_widget->ConvertFromRoot(x, y);
                        tb::TBWidgetEvent ev(tb::EVENT_TYPE_CONTEXT_MENU, x, y, false, modifier);
                        tb::TBWidget::hovered_widget->InvokeEvent(ev);
                }
        }
}

void GUI::glfw_callback_cursor_pos(GLFWwindow *window, double x, double y)
{
        mouse_x = (int)x;
        mouse_y = (int)y;
        if ( callback_widget ) 
            callback_widget->InvokePointerMove(mouse_x, mouse_y, GetModifierKeys(), false );
}

void GUI::glfw_callback_scroll(GLFWwindow *window, double x, double y)
{
    if ( callback_widget )
        callback_widget->InvokeWheel(mouse_x, mouse_y, (int)x, -(int)y, GetModifierKeys());
}
////////////////////////////////////////////////////////////////////////////////
//
//


} // namespace gui

} // namespace batb


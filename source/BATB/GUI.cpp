//    orienteering-game: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
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
#include "BATB/Log.hpp"
#include "tb/tb_core.h"
#include "tb/renderers/tb_renderer_gl.h"
#include "tb/tb_font_renderer.h"
#include "tb/animation/tb_animation.h"
#include "tb/animation/tb_widget_animation.h"
#include "tb/tb_system.h"
#include "tb/tb_msg.h"



// these are not part of tb::
//namespace tb { void register_tbbf_font_renderer(); }
//namespace tb { void register_stb_font_renderer(); }
//namespace tb { void register_freetype_font_renderer(); }
void register_tbbf_font_renderer();
void register_stb_font_renderer();
void register_freetype_font_renderer(); 


namespace BATB
{

// FIXME: TBWidget::focused_widget and multiple root widgets??
//        should probably not be a problem!
static tb::TBWidget* call_widget = 0;

static int mouse_x = 0;
static int mouse_y = 0;
static bool key_alt = false;
static bool key_ctrl = false;
static bool key_shift = false;
static bool key_super = false;


void GUI::create(xml::XMLElement* elem)
{
    using namespace xml;

    log << "GUI::create() " << std::endl;    


    XMLHandle xml( elem );
    // FIXME: parse xml...
  

// backend init:
//  * set up GLFW, set callbacks
//  * create TBRenderer
// tb_core_init: init TB
// set skin
// register font renderer, and init that
// set skin background for RootWidget
// DemoApplication::Init: 
//  * Application::Init: TBWidgetsAnimationManager::Init();
    tb_renderer_ =  new tb::TBRendererGL();
    tb::tb_core_init( tb_renderer_, File::staticData( "batb/gui/resources/language/lng_en.tb.txt" ).c_str() );

    // Load the default skin, and override skin that contains the graphics specific to the demo.
    tb::g_tb_skin->Load(  File::staticData( "batb/gui/resources/default_skin/skin.tb.txt" ).c_str(), 
                          File::staticData( "batb/gui/Demo/demo01/skin/skin.tb.txt").c_str() );

    // Register font renderers.
    // NOTE: for some reason, these are not part of namespace tb.
#ifdef TB_FONT_RENDERER_TBBF
    register_tbbf_font_renderer();
#endif
#ifdef TB_FONT_RENDERER_STB
    register_stb_font_renderer();
#endif
#ifdef TB_FONT_RENDERER_FREETYPE
    register_freetype_font_renderer();
#endif

    // Add fonts we can use to the font manager.
#if defined(TB_FONT_RENDERER_STB) || defined(TB_FONT_RENDERER_FREETYPE)
    tb::g_font_manager->AddFontInfo( File::staticData( "batb/gui/resources/vera.ttf", "Vera").c_str() );
#endif
#ifdef TB_FONT_RENDERER_TBBF
    tb::g_font_manager->AddFontInfo( File::staticData( "batb/gui/resources/default_font/segoe_white_with_shadow.tb.txt" ).c_str() , "Segoe");
    tb::g_font_manager->AddFontInfo( File::staticData( "batb/gui/Demo/fonts/neon.tb.txt" ).c_str(), "Neon" );
    tb::g_font_manager->AddFontInfo( File::staticData( "batb/gui/Demo/fonts/orangutang.tb.txt" ).c_str(), "Orangutang" );
    tb::g_font_manager->AddFontInfo( File::staticData( "batb/gui/Demo/fonts/orange.tb.txt" ).c_str(), "Orange" );
#endif

    // Set the default font description for widgets to one of the fonts we just added
    tb::TBFontDescription fd;
#ifdef TB_FONT_RENDERER_TBBF
    fd.SetID( tb::TBIDC("Segoe"));
#else
    fd.SetID( tb::TBIDC("Vera"));
#endif
    fd.SetSize( tb::g_tb_skin->GetDimensionConverter()->DpToPx(14));
    tb::g_font_manager->SetDefaultFontDescription(fd);

    //root_.SetSkinBg( tb::TBIDC("background") );

    // Create the font now.
    tb::TBFontFace *font = tb::g_font_manager->CreateFontFace( tb::g_font_manager->GetDefaultFontDescription());

    // Render some glyphs in one go now since we know we are going to use them. It would work fine
    // without this since glyphs are rendered when needed, but with some extra updating of the glyph bitmap.
    if (font)
            font->RenderGlyphs(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
                                                    "â‚¬â€šÆ’â€žâ€¦â€ â€¡Ë†â€°Å â€¹Å’Å½â€˜â€™â€œâ€â€¢â€“â€”Ëœâ„¢Å¡â€ºÅ“Å¾Å¸Â¡Â¢Â£Â¤Â¥Â¦Â§Â¨Â©ÂªÂ«Â¬Â®"
                                                    "Â¯Â°Â±Â²Â³Â´ÂµÂ¶Â·Â¸Â¹ÂºÂ»Â¼Â½Â¾Â¿Ã€ÃÃ‚ÃƒÃ„Ã…Ã†Ã‡ÃˆÃ‰ÃŠÃ‹ÃŒÃÃŽÃÃÃ‘Ã’Ã“Ã”Ã•Ã–Ã—Ã˜Ã™ÃšÃ›ÃœÃÃžÃŸÃ Ã"
                                                    "¡Ã¢Ã£Ã¤Ã¥Ã¦Ã§Ã¨Ã©ÃªÃ«Ã¬Ã­Ã®Ã¯Ã°Ã±Ã²Ã³Ã´ÃµÃ¶Ã·Ã¸Ã¹ÃºÃ»Ã¼Ã½Ã¾Ã¿");

    // Give the root widget a background skin
    //application->GetRoot()->SetSkinBg(TBIDC("background"));

    tb::TBWidgetsAnimationManager::Init();


    // callback widget
    call_widget = &root_;
}


void GUI::destroy()
{
    log << "GUI::destroy() " << std::endl;    

    tb::TBWidgetsAnimationManager::Shutdown();

    tb::tb_core_shutdown();

    delete tb_renderer_;
    tb_renderer_ = nullptr;

}


// FIXME: argument??
void GUI::output(uint wth, uint hth)
{

//      g_renderer->BeginPaint(window_w, window_h);
//      GetRoot()->InvokePaint(TBWidget::PaintProps());
//      g_renderer->EndPaint();
//      
//      FIXME: not here:
        root_.SetRect( tb::TBRect( 0, 0, wth, hth ) );

        tb::g_renderer->BeginPaint( wth, hth );
        root_.InvokePaint( tb::TBWidget::PaintProps() );
        tb::g_renderer->EndPaint();
//      Root()->Invalidate(); // ?
}

// FIXME: argument??
void GUI::step()
{
    // FIXME: Scene
    uint wth, hth;
    Env::screenSize( wth, hth );
    root_.SetRect( tb::TBRect(0, 0, wth, hth));

    // FIXME: set variables for custom TBSystem implementation

    // update messages for TB, that is, send TBMessage's from
    // the global message queue to the receiving TBMessageHandler
    // for each message
    tb::TBMessageHandler::ProcessMessages(); 

    tb::TBAnimationManager::Update();
    root_.InvokeProcessStates();
    root_.InvokeProcess();
    
}

    static void glfw_callback_char(GLFWwindow *window, unsigned int character);
    static void glfw_callback_key(GLFWwindow *window, int key, int scancode, int action, int glfwmod);
    static void glfw_callback_mouse_button(GLFWwindow *window, int button, int action, int glfwmod);
    static void glfw_callback_cursor_pos(GLFWwindow *window, double x, double y);
    static void glfw_callback_scroll(GLFWwindow *window, double x, double y);
    static void glfw_callback_window_size(GLFWwindow *window, int w, int h);

void GUI::keysCallback()
{

    // FIXME: keys::glfw_callback_window_size( glfw_callback_window_size );
    //        ...
    //
	glfwSetWindowSizeCallback(Env::screenWindow(), glfw_callback_window_size );
	//glfwSetWindowRefreshCallback(Env::screenWindow(), );
	glfwSetCursorPosCallback(Env::screenWindow(), glfw_callback_cursor_pos );
    glfwSetMouseButtonCallback(Env::screenWindow(), glfw_callback_mouse_button );
    glfwSetScrollCallback(Env::screenWindow(), glfw_callback_scroll );
    glfwSetKeyCallback(Env::screenWindow(), glfw_callback_key );
    glfwSetCharCallback(Env::screenWindow(), glfw_callback_char );
    //glfwSetTimerCallback(timer_callback);
#if (GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 1)
	//glfwSetDropCallback(mainWindow, drop_callback);
#endif

}

//
// @return Return the upper case of a ascii charcter. Only for shortcut handling.
static int toupr_ascii(int ascii)
{
	if (ascii >= 'a' && ascii <= 'z')
		return ascii + 'A' - 'a';
	return ascii;
}


tb::MODIFIER_KEYS GetModifierKeys()
{
	tb::MODIFIER_KEYS code = tb::TB_MODIFIER_NONE;
	if (key_alt)	code |= tb::TB_ALT;
	if (key_ctrl)	code |= tb::TB_CTRL;
	if (key_shift)	code |= tb::TB_SHIFT;
	if (key_super)	code |= tb::TB_SUPER;
	return code;
}

tb::MODIFIER_KEYS GetModifierKeys(int modifier)
{
	tb::MODIFIER_KEYS code = tb::TB_MODIFIER_NONE;
	if (modifier & GLFW_MOD_ALT)		code |= tb::TB_ALT;
	if (modifier & GLFW_MOD_CONTROL)	code |= tb::TB_CTRL;
	if (modifier & GLFW_MOD_SHIFT)		code |= tb::TB_SHIFT;
	if (modifier & GLFW_MOD_SUPER)		code |= tb::TB_SUPER;
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
		id = tb::TBIDC("cut");
	else if ((toupr_ascii(key) == 'C' || special_key == tb::TB_KEY_INSERT) && shortcut_key)
		id = tb::TBIDC("copy");
	else if (((toupr_ascii(key) == 'V' && shortcut_key) ||
			(special_key == tb::TB_KEY_INSERT && reverse_key)))
		id = tb::TBIDC("paste");
	else if (toupr_ascii(key) == 'A' && shortcut_key)
		id = tb::TBIDC("selectall");
	else if ((toupr_ascii(key) == 'Z' && shortcut_key) ||
			(toupr_ascii(key) == 'Y' && shortcut_key))
	{
		bool undo = toupr_ascii(key) == 'Z';
		if (reverse_key)
			undo = !undo;
		id = undo ? tb::TBIDC("undo") : tb::TBIDC("redo");
	}
	else
		return false;

	tb::TBWidgetEvent ev(tb::EVENT_TYPE_SHORTCUT);
	ev.modifierkeys = modifierkeys;
	ev.ref_id = id;
	return tb::TBWidget::focused_widget->InvokeEvent(ev);
}

static bool InvokeKey(GLFWwindow *window, unsigned int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down)
{
	if (InvokeShortcut(key, tb::TB_KEY_UNDEFINED, modifierkeys, down))
		return true;
	//GetBackend(window)->GetRoot()->InvokeKey(key, special_key, modifierkeys, down);
        if ( call_widget ) call_widget->InvokeKey( key, special_key, modifierkeys, down );
	return true;
}
static void glfw_callback_char(GLFWwindow *window, unsigned int character)
{
	// glfw on osx seems to send us characters from the private
	// use block when using f.ex arrow keys on osx.
	if (character >= 0xE000 && character <= 0xF8FF)
		return;

	InvokeKey(window, character, tb::TB_KEY_UNDEFINED, GetModifierKeys(), true);
	InvokeKey(window, character, tb::TB_KEY_UNDEFINED, GetModifierKeys(), false);
}
static void glfw_callback_key(GLFWwindow *window, int key, int scancode, int action, int glfwmod)
{
	tb::MODIFIER_KEYS modifier = GetModifierKeys(glfwmod);
	bool down = (action == GLFW_PRESS || action == GLFW_REPEAT);
	switch (key)
	{
	case GLFW_KEY_F1:			InvokeKey(window, 0, tb::TB_KEY_F1, modifier, down); break;
	case GLFW_KEY_F2:			InvokeKey(window, 0, tb::TB_KEY_F2, modifier, down); break;
	case GLFW_KEY_F3:			InvokeKey(window, 0, tb::TB_KEY_F3, modifier, down); break;
	case GLFW_KEY_F4:			InvokeKey(window, 0, tb::TB_KEY_F4, modifier, down); break;
	case GLFW_KEY_F5:			InvokeKey(window, 0, tb::TB_KEY_F5, modifier, down); break;
	case GLFW_KEY_F6:			InvokeKey(window, 0, tb::TB_KEY_F6, modifier, down); break;
	case GLFW_KEY_F7:			InvokeKey(window, 0, tb::TB_KEY_F7, modifier, down); break;
	case GLFW_KEY_F8:			InvokeKey(window, 0, tb::TB_KEY_F8, modifier, down); break;
	case GLFW_KEY_F9:			InvokeKey(window, 0, tb::TB_KEY_F9, modifier, down); break;
	case GLFW_KEY_F10:			InvokeKey(window, 0, tb::TB_KEY_F10, modifier, down); break;
	case GLFW_KEY_F11:			InvokeKey(window, 0, tb::TB_KEY_F11, modifier, down); break;
	case GLFW_KEY_F12:			InvokeKey(window, 0, tb::TB_KEY_F12, modifier, down); break;
	case GLFW_KEY_LEFT:			InvokeKey(window, 0, tb::TB_KEY_LEFT, modifier, down); break;
	case GLFW_KEY_UP:			InvokeKey(window, 0, tb::TB_KEY_UP, modifier, down); break;
	case GLFW_KEY_RIGHT:		InvokeKey(window, 0, tb::TB_KEY_RIGHT, modifier, down); break;
	case GLFW_KEY_DOWN:			InvokeKey(window, 0, tb::TB_KEY_DOWN, modifier, down); break;
	case GLFW_KEY_PAGE_UP:		InvokeKey(window, 0, tb::TB_KEY_PAGE_UP, modifier, down); break;
	case GLFW_KEY_PAGE_DOWN:	InvokeKey(window, 0, tb::TB_KEY_PAGE_DOWN, modifier, down); break;
	case GLFW_KEY_HOME:			InvokeKey(window, 0, tb::TB_KEY_HOME, modifier, down); break;
	case GLFW_KEY_END:			InvokeKey(window, 0, tb::TB_KEY_END, modifier, down); break;
	case GLFW_KEY_INSERT:		InvokeKey(window, 0, tb::TB_KEY_INSERT, modifier, down); break;
	case GLFW_KEY_TAB:			InvokeKey(window, 0, tb::TB_KEY_TAB, modifier, down); break;
	case GLFW_KEY_DELETE:		InvokeKey(window, 0, tb::TB_KEY_DELETE, modifier, down); break;
	case GLFW_KEY_BACKSPACE:	InvokeKey(window, 0, tb::TB_KEY_BACKSPACE, modifier, down); break;
	case GLFW_KEY_ENTER:		InvokeKey(window, 0, tb::TB_KEY_ENTER, modifier, down); break;
	case GLFW_KEY_ESCAPE:		InvokeKey(window, 0, tb::TB_KEY_ESC, modifier, down); break;
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
	default:
		// glfw calls key_callback instead of char_callback
		// when pressing a character while ctrl is also pressed.
		if (key_ctrl && !key_alt && key >= 32 && key <= 255)
			InvokeKey(window, key, tb::TB_KEY_UNDEFINED, modifier, down);
		break;
	}
}
static void glfw_callback_mouse_button(GLFWwindow *window, int button, int action, int glfwmod)
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
                        tick_t time = Env::tick();

			if (time < last_time + 600 && last_x == x && last_y == y)
				counter++;
			else
				counter = 1;
			last_x = x;
			last_y = y;
			last_time = time;

			//GetBackend(window)->GetRoot()->InvokePointerDown(x, y, counter, modifier, ShouldEmulateTouchEvent());
			if ( call_widget ) call_widget->InvokePointerDown(x, y, counter, modifier, false );
		}
		else
			//GetBackend(window)->GetRoot()->InvokePointerUp(x, y, modifier, ShouldEmulateTouchEvent());
			if ( call_widget ) call_widget->InvokePointerUp(x, y, modifier,  false );
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		//GetBackend(window)->GetRoot()->InvokePointerMove(x, y, modifier, ShouldEmulateTouchEvent());
		if ( call_widget ) call_widget->InvokePointerMove(x, y, modifier, false );
		if (tb::TBWidget::hovered_widget)
		{
			tb::TBWidget::hovered_widget->ConvertFromRoot(x, y);
			tb::TBWidgetEvent ev(tb::EVENT_TYPE_CONTEXT_MENU, x, y, false, modifier);
			tb::TBWidget::hovered_widget->InvokeEvent(ev);
		}
	}
}
static void glfw_callback_cursor_pos(GLFWwindow *window, double x, double y)
{
	mouse_x = (int)x;
	mouse_y = (int)y;
	//if (GetBackend(window)->GetRoot() && !(ShouldEmulateTouchEvent() && !tb::TBWidget::captured_widget))
		//GetBackend(window)->GetRoot()->InvokePointerMove(mouse_x, mouse_y, GetModifierKeys(), ShouldEmulateTouchEvent());
		if ( call_widget ) call_widget->InvokePointerMove(mouse_x, mouse_y, GetModifierKeys(), false );
}
static void glfw_callback_scroll(GLFWwindow *window, double x, double y)
{
	//if (GetBackend(window)->GetRoot())
		//GetBackend(window)->GetRoot()->InvokeWheel(mouse_x, mouse_y, (int)x, -(int)y, GetModifierKeys());
		if ( call_widget ) call_widget->InvokeWheel(mouse_x, mouse_y, (int)x, -(int)y, GetModifierKeys());
}
static void glfw_callback_window_size(GLFWwindow *window, int w, int h)
{
	//ApplicationBackendGLFW *backend = GetBackend(window);
	//if (backend->GetRoot())
		//backend->GetRoot()->SetRect(tb::TBRect(0, 0, w, h));
                if ( call_widget ) call_widget->SetRect( tb::TBRect( 0, 0, w, h ) );
}


}


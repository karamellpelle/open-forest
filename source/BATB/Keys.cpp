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
#include "BATB/Keys.hpp"
#include "BATB/Keys/KeyKeyboardButton.hpp"
#include "BATB/Keys/KeyMouseButton.hpp"
#include "BATB/Keys/KeyMouseAxis.hpp"
#include "BATB/Keys/KeyMouseScroll.hpp"
#include "BATB/Keys/KeyClicker.hpp"
#include "BATB/Keys/KeyAlpha.hpp"
#include "BATB/Keys/KeyPointer.hpp"
#include "BATB/Screen.hpp"
#include "helpers/glfw.hpp"



namespace batb
{

namespace keys
{

////////////////////////////////////////////////////////////////////////////////
// these creates glfw codes (int) from string
// map names into GLFW mouse button
//
code::MouseButton map_mousecode(const std::string& name)
{
    return glfw_map_mousebutton( name );
}


// map names into GLFW keyboar key
code::KeyboardButton map_keyboardcode(const std::string& name)
{
    return glfw_map_keyboardbutton( name );
}


////////////////////////////////////////////////////////////////////////////////
// the kananical Keys (needed for scroll)
Keys* Keys::keysobj_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
// setup 

void Keys::begin(const std::string& path)
{
    batb->log << "batb->keys->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
        // set configuration file
        config( path );

        // retrieve GLFW window (needed for input)
        glfw_window_ = batb->screen->glfw_window;

        // cursor input mode is _NORMAL as default
        glfwSetInputMode( glfw_window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL ); 
        cursor_free_ = false;

        // let scroll callback be able to reach this object (implies only 1 Keys in program)
        keysobj_ = this;
        glfwSetScrollCallback( glfw_window_, scroll_callback_ );
    }

    init( true );
}

void Keys::end()
{
    batb->log << "batb->keys->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        // save configuration
        save();

        // remove all created Key's
        clear();
    }

    init( false );
}
////////////////////////////////////////////////////////////////////////////////
//

// remove all created Key's 
void Keys::clear()
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), std::default_delete<Key>() );
    keys_.clear();

    // also clear scroll's
    scrolls_x_.clear();
    scrolls_y_.clear();
}


// reset all Key's
void Keys::reset()
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), [](auto k) { k->reset(); } );
}

// update all keys, to be done at each frame:
void Keys::step(tick_t t)
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), [t](auto k) { k->step( t ); } );
}



////////////////////////////////////////////////////////////////////////////////
//

void Keys::keyEnable(bool enable)
{
    if ( key_enable_ != enable )
    {
        if ( enable )
        {
            glfwSetCursorPos( glfw_window_, cursor_enable_x0_, cursor_enable_y0_ );
        }
        else
        {
            // save current state (i.e. cursor pos)
            double x1, y1;
            glfwGetCursorPos( glfw_window_, &x1, &y1 );

            cursor_enable_x0_ = x1;
            cursor_enable_y0_ = y1;

            // also previous cursor pos
            cursor_x0_ = x1;
            cursor_y0_ = y1;
        }
    }

    key_enable_ = enable;
}


////////////////////////////////////////////////////////////////////////////////
//

// get keyboard button state
code::ButtonState Keys::getKey(code::KeyboardButton k)
{ 
    return glfwGetKey( glfw_window_, k );
}

// get mouse button state
code::ButtonState Keys::getMouseButton(code::MouseButton k)
{ 
    return glfwGetMouseButton( glfw_window_, k );
}

code::ButtonState Keys::getKey_(code::KeyboardButton k)                          
{
    return key_enable_ ? getKey( k ) : GLFW_RELEASE;
}  

code::ButtonState Keys::getMouseButton_(code::MouseButton k)
{ 
    return key_enable_ ? getMouseButton( k ) : GLFW_RELEASE; 
}

////////////////////////////////////////////////////////////////////////////////

// get cursor pos, in pixels
void Keys::getCursorPos(int& x, int& y)
{
    double x_, y_;
    glfwGetCursorPos( glfw_window_, &x_, &y_ );
    x = (int)( std::floor( x_ ) );
    y = (int)( std::floor( y_ ) );
}

void Keys::setCursorPos(int x, int y)
{
    glfwSetCursorPos( glfw_window_, (double)( x ), (double)( y ) );
}

// get cursor pos, relative to screen shape 
void Keys::getCursorPos(double& x, double& y)
{
    int wth; int hth;
    glfwGetWindowSize( glfw_window_, &wth, &hth );

    double x_, y_;
    glfwGetCursorPos( glfw_window_, &x_, &y_ );

    double scale = 1.0 / (double)( std::max( wth, hth ) );
    x = (double)( x_ ) * scale;
    y = (double)( y_ ) * scale;
}

// set cursor pos, relative to screen shape 
void Keys::setCursorPos(double x, double y)
{
    int wth; int hth;
    glfwGetWindowSize( glfw_window_, &wth, &hth );
    double unit = (double)( std::max( wth, hth ) );
   
    glfwSetCursorPos( glfw_window_, x * unit, y * unit );
}


void Keys::getCursorPos_(int& x, int& y)
{
    if ( key_enable_ )
    {
        getCursorPos( x, y );
    }
    else
    {
        x = (int)( std::floor( cursor_enable_x0_ ) );
        y = (int)( std::floor( cursor_enable_y0_ ) );
    }
}

void Keys::setCursorPos_(int x, int y)
{
    if ( key_enable_ )
    {
        setCursorPos( x, y );
    }
}

void Keys::getCursorPos_(double& x, double& y)
{
    if ( key_enable_ )
    {
        glfwGetCursorPos( glfw_window_, &x, &y );
    }
    else
    {
        x = cursor_enable_x0_;
        y = cursor_enable_y0_;
    }

    int wth; int hth;
    glfwGetWindowSize( glfw_window_, &wth, &hth ); 
    double scale = 1.0 / (double)( std::max( wth, hth ) );
    x *=  scale;
    y *=  scale;
}

void Keys::setCursorPos_(double x, double y)
{
    if ( key_enable_ )
    {
        setCursorPos( x, y );
    }
}

void Keys::setCursorFree(bool free)
{
    if ( cursor_free_ != free )
    {
        // get current
        double x1, y1;
        glfwGetCursorPos( glfw_window_, &x1, &y1 );

        
        glfwSetInputMode( glfw_window_, GLFW_CURSOR, 
            free ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL ); 
            // ^ FIXME: _NORMAL/_HIDDEN based on setting!

        // set back to previous
        glfwSetCursorPos( glfw_window_, cursor_x0_, cursor_y0_ );

        // save previous
        cursor_x0_ = x1;
        cursor_y0_ = y1;

    }
    cursor_free_ = free;
}

void Keys::charCalling(GLFWcharfun f)
{
    glfwSetCharCallback( glfw_window_, f );
}

void Keys::keyCalling(GLFWkeyfun f)
{
    glfwSetKeyCallback( glfw_window_, f );
}

void Keys::mousebuttonCalling(GLFWmousebuttonfun f)
{
    glfwSetMouseButtonCallback( glfw_window_, f );
}

void Keys::cursorposCalling(GLFWcursorposfun f)
{
    glfwSetCursorPosCallback( glfw_window_, f );
}

void Keys::scrollCalling(GLFWscrollfun f)
{
    scroll_cbks_.push_back( f );
}


////////////////////////////////////////////////////////////////////////////////
// create Key's from YAML::Node
//


Key* Keys::createKey(const YAML::Node& yaml)
{
    using namespace YAML;
    //std::cout << "createKey()" << std::endl << yaml << std::endl << std::endl;
  
    if ( !yaml )
    {
        batb->log << "ERROR: YAML::Node does not exist";
        return nullptr;
    }
    if ( !yaml.IsMap() )
    {
        batb->log << "ERROR: wrong type of YAML::Node (" << yaml.Type() << ")"; 
        return nullptr;
    }

    // which kind of Key?
    if ( Node key = yaml["KeyKeyboardButton"] ) return createKeyKeyboardButton( key );
    if ( Node key = yaml["KeyMouseButton"] )    return createKeyMouseButton( key );
    if ( Node key = yaml["KeyMouseAxis"] )      return createKeyMouseAxis( key );
    if ( Node key = yaml["KeyMouseScroll"] )    return createKeyMouseScroll( key );
    if ( Node key = yaml["KeyClicker"] )        return createKeyClicker( key );
    if ( Node key = yaml["KeyAlpha"] )          return createKeyAlpha( key );
    if ( Node key = yaml["KeyPointer"] )        return createKeyPointer( key );


    batb->log << "ERROR: unknown Key";
    return nullptr;
}

Key* Keys::createSafeKey(const YAML::Node& yaml)
{
    // using the one above
    auto key = createKey( yaml );
    if ( key ) return key;

    batb->log << "! WARNING: using empty constructor instead";

    // create from empty constructor
    return create<Key>();
}


KeyKeyboardButton* Keys::createKeyKeyboardButton(const YAML::Node& yaml)
{
    if ( !yaml )
    {
        batb->log << "ERROR: YAML::Node does not exist";
        return nullptr;
    }
    if ( !yaml.IsMap() )
    {
        batb->log << "ERROR: wrong type of YAML::Node (" << yaml.Type() << ")"; 
        return nullptr;
    }

    auto code_str = yaml["code"].as<std::string>( "" );
    if ( code_str.empty() )
    {
        batb->log << "ERROR: no 'code' field in KeyKeyboardButton"; 
        return nullptr;
    }
    
    auto glfw_code =  map_keyboardcode( code_str ) ;
    auto ret = create<KeyKeyboardButton>( glfw_code );
    batb->log << "KeyKeyboardButton: { code: ";
    if   ( glfw_code == GLFW_KEY_UNKNOWN ) batb->log << "UNKNOWN (" << code_str << ")"; // maybe this should be ERROR instead?
    else                                   batb->log << code_str;
    batb->log << " }";

    return ret;
}

KeyMouseButton* Keys::createKeyMouseButton(const YAML::Node& yaml)
{
    if ( !yaml )
    {
        batb->log << "ERROR: YAML::Node does not exist";
        return nullptr;
    }
    if ( !yaml.IsMap() )
    {
        batb->log << "ERROR: wrong type of YAML::Node (" << yaml.Type() << ")"; 
        return nullptr;
    }

    auto code_str = yaml["code"].as<std::string>( "" );
    if ( code_str.empty() )
    {
        batb->log << "ERROR: no 'code' field in KeyMouseButton"; 
        return nullptr;
    }
    
    auto glfw_code =  map_mousecode( code_str ) ;
    auto ret = create<KeyMouseButton>( glfw_code );
    batb->log << "KeyMouseButton: { code: ";
    if   ( glfw_code == GLFW_KEY_UNKNOWN ) batb->log << "UNKNOWN (" << code_str << ")"; // maybe this should be ERROR instead?
    else                                   batb->log << code_str;
    batb->log << " }";

    return ret;
}

KeyMouseAxis* Keys::createKeyMouseAxis(const YAML::Node& yaml)
{
    if ( !yaml )
    {
        batb->log << "ERROR: YAML::Node does not exist";
        return nullptr;
    }
    if ( !yaml.IsMap() )
    {
        batb->log << "ERROR: wrong type of YAML::Node (" << yaml.Type() << ")"; 
        return nullptr;
    }

    auto str = yaml["axis"].as<std::string>( "" );
    if ( str.empty() )
    {
        batb->log << "ERROR: no 'axis' field in KeyMouseAxis"; 
        return nullptr;
    }
    
    if ( str == "X" )
    {
        batb->log << "KeyMouseAxis: { axis: X }";
        return create<KeyMouseAxis>( code::MouseAxis::X );
    }
    if ( str == "Y" )
    {
        batb->log << "KeyMouseAxis: { axis: Y }";
        return create<KeyMouseAxis>( code::MouseAxis::Y );
    }

    batb->log << "ERROR: no valid 'axis' field (" << str << ") in KeyMouseAxis";
    return nullptr;

}

KeyMouseScroll* Keys::createKeyMouseScroll(const YAML::Node& yaml)
{
    if ( !yaml )
    {
        batb->log << "ERROR: YAML::Node does not exist";
        return nullptr;
    }
    if ( !yaml.IsMap() )
    {
        batb->log << "ERROR: wrong type of YAML::Node (" << yaml.Type() << ")"; 
        return nullptr;
    }
      
    auto str = yaml["axis"].as<std::string>( "" );
    if ( str.empty() )
    {
        batb->log << "ERROR: no 'axis' field in KeyMouseScroll"; 
        return nullptr;
    }
    
    if ( str == "X" )
    {
        batb->log << "KeyMouseScroll: { axis: X }";
        return create<KeyMouseScroll>( code::MouseScroll::X ); 
    }
    if ( str == "Y" )
    {
        batb->log << "KeyMouseScroll: { axis: Y }";
        return create<KeyMouseScroll>( code::MouseScroll::Y ); 
    }

    batb->log << "ERROR: no valid 'axis' field (" << str << ") in KeyMouseScroll";
    return nullptr;

}
KeyClicker* Keys::createKeyClicker(const YAML::Node& yaml)
{
    
    auto child0 = createKey( yaml ); 
    batb->log << ", in KeyMouseClicker";

    return create<KeyClicker>( child0 );
}

KeyAlpha* Keys::createKeyAlpha(const YAML::Node& yaml)
{
    auto child0 = createKey( yaml ); 
    batb->log << ", in KeyMouseAlpha";

    return create<KeyAlpha>( child0 );

}
KeyPointer* Keys::createKeyPointer(const YAML::Node& yaml)
{
    if ( !yaml )
    {
        batb->log << "ERROR: YAML::Node does not exist";
        return nullptr;
    }
    if ( !yaml.IsMap() )
    {
        batb->log << "ERROR: wrong type of YAML::Node (" << yaml.Type() << ")"; 
        return nullptr;
    }
    // empty pointer OK, use default
    Key* axis_x = nullptr;
    Key* axis_y = nullptr;
    Key* left = nullptr;
    Key* right = nullptr;

    // axis-x
    batb->log << "[ ";
    if ( auto child = yaml["axis-x"] )  axis_x = createKey( child );
    if ( !axis_x )                      batb->log << " (using default axis_x)"; 
    batb->log << " ]";

    // axis-x
    batb->log << "[ ";
    if ( auto child = yaml["axis-y"] )  axis_y = createKey( child );
    if ( !axis_y )                      batb->log << " (using default axis_y)"; 
    batb->log << " ]";

    // left
    batb->log << "[ ";
    if ( auto child = yaml["left"] )  left = createKey( child );
    if ( !left )                      batb->log << " (using default left)"; 
    batb->log << " ]";

    // right
    batb->log << "[ ";
    if ( auto child = yaml["right"] )  right = createKey( child );
    if ( !right )                      batb->log << " (using default right)"; 
    batb->log << " ]";

    batb->log << ", in KeyPointer";
    return create<KeyPointer>( axis_x, axis_y, left, right );
}

////////////////////////////////////////////////////////////////////////////////
//
void Keys::scroll_callback_(GLFWwindow* win, double x, double y)
{
    
    //std::cout << "scroll! " << x << ", " << y << std::endl;
    
    // callback every registered callback
    for ( auto cbk : Keys::keysobj_->scroll_cbks_ )
    {
        cbk( win, x, y );
    }

    // call our KeyMouseScroll's
    for ( auto scroll : Keys::keysobj_->scrolls_x_ )
    {
       // TODO: if enabled  
       scroll->add( x );
    }
    for ( auto scroll : Keys::keysobj_->scrolls_y_ )
    {
       // TODO: if enabled  
       scroll->add( y );
    }

}


} // namespace keys

} // namespace batb


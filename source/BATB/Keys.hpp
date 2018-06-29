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
#ifndef BATB_KEYS_HPP
#define BATB_KEYS_HPP
#include "BATB/ModuleBATB.hpp"
#include "BATB/Keys/Code.hpp"



namespace batb
{
namespace keys
{


class Key;
class KeyKeyboardButton;      
class KeyMouseButton; 
class KeyMouseAxis;  
class KeyMouseScroll;
class KeyClicker;     
class KeyAlpha;       
class KeyPointer;     


////////////////////////////////////////////////////////////////////////////////
// Keys: control input 
//  - keyboard
//  - mouse
//  - joystick (todo)
//  - etc.
// currently using GLFW as backend
//
class Keys : public ModuleBATB
{

public:
    Keys(BATB* b) : ModuleBATB( b ) { }

    // setup this module
    void begin(const std::string& );
    void end();

    ////////////////////////////////////////////////////////////////////////////////
    // modify state of this object

    // remove all created Key's
    void clear();
    // reset state for all created Key's
    void reset();
    // update all created Key's. to be done at each frame
    void step(tick_t t);

    ////////////////////////////////////////////////////////////////////////////////
    // working with keys
    
    // WTF does this? i dont remember. enable/disable cursor? 
    void keyEnable(bool b);

    // get cursor pos, in pixels
    void getCursorPos(int& x, int& y);
    // get cursor pos, relative to screen shape 
    void getCursorPos(double& x, double& y);
    // set cursor pos, in pixels
    void setCursorPos(int x, int y);
    // set cursor pos, relative to screen shape
    void setCursorPos(double x, double y);
    // can cursor (in window/pixel coordinates) move outside of window, that is,
    // infinite movement but no pointer is shown? default: false
    void setCursorFree(bool ); 

    // get keyboard button state
    code::ButtonState getKey(code::KeyboardButton );
    // get mouse button state
    code::ButtonState getMouseButton(code::MouseButton k);

    ////////////////////////////////////////////////////////////////////////////////
    // set (GLFW) callback functions
    void charCalling(GLFWcharfun );
    void keyCalling(GLFWkeyfun );
    void mousebuttonCalling(GLFWmousebuttonfun );
    void cursorposCalling(GLFWcursorposfun );
    void scrollCalling(GLFWscrollfun );

    ////////////////////////////////////////////////////////////////////////////////
    // let's create a Key!
    //

    // create by constructor (=> K is subclass of Key)
    template <typename K, typename... Args>
    K*                 create(Args... );
    // create by constructor, but safe, that is, return the one with default constructor
    // instead of nullptr if fail
    template <typename K, typename... Args>
    K*                 createSafe(Args... );

    ////////////////////////////////////////////////////////////////

    // create Key from definition
    // (I was not able to create these as template specializations of the two above
    // functions. c++ templates are a mess :( )
    Key* createKey(const YAML::Node& );
    Key* createSafeKey(const YAML::Node& );


    // TODO: 
    //  * implement this when the user can change control mappings in real time
    //  * if using shared_ptr<Key> we must be sure we don't remove it if is used
    //    other places, since removing it will prevent it from being stepped
    void               removeKey(Key* ) { }

    ////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////
    // these are to be used by Key's (checks if Key's disabled or not)
    void getCursorPos_(int& x, int& y);
    void getCursorPos_(double& x, double& y);
    void setCursorPos_(int x, int y);
    void setCursorPos_(double x, double y);
    code::ButtonState getKey_(code::KeyboardButton );    
    code::ButtonState getMouseButton_(code::MouseButton );

private:
    // these helps us parse YAML nodes
    // prims
    //Key*               createKey(const YAML::Node& );     
    KeyKeyboardButton* createKeyKeyboardButton(const YAML::Node& );
    KeyMouseButton*    createKeyMouseButton(const YAML::Node& );
    KeyMouseAxis*      createKeyMouseAxis(const YAML::Node& );
    KeyMouseScroll*    createKeyMouseScroll(const YAML::Node& );
    // cons
    KeyClicker*        createKeyClicker(const YAML::Node& );
    KeyAlpha*          createKeyAlpha(const YAML::Node& );
    KeyPointer*        createKeyPointer(const YAML::Node& );

    // the input context (GLFW window)
    GLFWwindow* glfw_window_ = nullptr;

    // enable/disable all Key's. TODO
    bool key_enable_ = true;

    // previous cursor
    double cursor_enable_x0_ = 0;
    double cursor_enable_y0_ = 0;
    bool cursor_free_ = false;
    double cursor_x0_ = 0;
    double cursor_y0_ = 0;

    ////////////////////////////////////////////////////////////////////////////////
    // Key's
    //
    using KeyContainer = std::vector<Key*>; // TODO: std::list because of add/remove Key's
    KeyContainer keys_;

    // handle GLFW scroll
    static Keys* keysobj_;
    static void scroll_callback_(GLFWwindow* , double , double );
    std::vector<GLFWscrollfun> scroll_cbks_;
    std::vector<KeyMouseScroll*> scrolls_x_;
    std::vector<KeyMouseScroll*> scrolls_y_;
};



////////////////////////////////////////////////////////////////////////////////
//  

// 

template <typename K, typename... Args>
inline K* Keys::create(Args... args)
{
    K* ret = new K( batb, args... );
    keys_.push_back( ret ); 
    return ret; 
}


// KeyMouseScroll needs special care; make a template specialization
template <>
inline KeyMouseScroll* Keys::create<KeyMouseScroll>(code::MouseScroll c)
{
    auto ret = create<KeyMouseScroll>( c );

    // also add to scrolls
    if ( c == code::MouseScroll::X )
    {
        scrolls_x_.push_back( ret );
    }
    if ( c == code::MouseScroll::Y )
    {
        scrolls_y_.push_back( ret );
    }
    // (unknown code ignored, but still added)

    return ret;
}


////////////////////////////////////////////////////////////////////////////////

// create by constructor, but safe, that is, return the one with default cons-
// tructor if failure
template <typename K, typename... Args>
inline K* Keys::createSafe(Args... args)
{
    auto key = create<K>( args... );
    if ( key ) return key;

    batb->log << "! WARNING: using empty constructor instead";

    // create 
    return create<K>();
}


////////////////////////////////////////////////////////////////////////////////




} // namespace keys

} // namespace batb


#endif

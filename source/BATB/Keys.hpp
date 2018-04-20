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
#include <GLFW/glfw3.h>

namespace batb
{
namespace keys
{


class Key;
class KeyButton;      
class KeyMouseButton; 
class KeyMouseAxisX;  
class KeyMouseAxisY;  
class KeyMouseScroll;
class KeyClicker;     
class KeyAlpha;       
class KeyPointer;     


// class Keys: input from keyboard, mouse, joystick, etc.
// currently bound to GLFW!
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
    // TODO: remove key. make sure to look in scrolls_x/y too
    //void remove(Key* );
    // reset state for all created Key's
    void reset();
    // update all created Key's. to be done at each frame
    void step(tick_t t);

    ////////////////////////////////////////////////////////////////////////////////
    // working with keys

    // WTF does this? i dont remember. enable/disable cursor? 
    void keyEnable(bool b);
    // get cursor pos, in pixels
    void getCursorPos(uint& x, uint& y);
    // get cursor pos, relative to screen shape 
    void getCursorPos(double& x, double& y);
    // can cursor move outside of window?
    void setCursorFree(bool ); // default false
    // get keyboard button state
    int getKey(int k) { return glfwGetKey( window_, k ); }
    // get mouse button state
    int getMouseButton(int k) { return glfwGetMouseButton( window_, k ); }

    ////////////////////////////////////////////////////////////////////////////////
    // set callback functions
    void charCalling(GLFWcharfun );
    void keyCalling(GLFWkeyfun );
    void mousebuttonCalling(GLFWmousebuttonfun );
    void cursorposCalling(GLFWcursorposfun );
    void scrollCalling(GLFWscrollfun );

    ////////////////////////////////////////////////////////////////////////////////
    // let's create a Key!
    //
    // prims
    KeyButton*      createKeyButton(int code);
    KeyMouseButton* createKeyMouseButton(int code);
    KeyMouseAxisX*  createKeyMouseAxisX();
    KeyMouseAxisY*  createKeyMouseAxisY();
    KeyMouseScroll*  createKeyMouseScrollX();
    KeyMouseScroll*  createKeyMouseScrollY();
    // cons
    KeyClicker*     createKeyClicker(Key* k);
    KeyAlpha*       createKeyAlpha(Key* k);
    KeyPointer*     createKeyPointer(Key* x, Key* y, Key* l, Key* r);
    KeyPointer*     createKeyPointer();
    // create Key from definition
    Key* createKey(const YAML::Node& );


    ////////////////////////////////////////////////////////////////////////////////
    // these are to be used by Key's (checks if Key's disabled or not)
    void getCursorPos_(uint& x, uint& y);
    void getCursorPos_(double& x, double& y);
    int getKey_(int k)                          { if ( key_enable_ ) return getKey( k ); else return GLFW_RELEASE;  }  
    int getMouseButton_(int k)                  { if ( key_enable_ ) return getMouseButton( k ); else return GLFW_RELEASE; }

private:

    // we need 1 Keys for scroll callback
    static Keys* keysobj_;

    GLFWwindow* window_ = nullptr;

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
    using KeyContainer = std::vector<Key*>; // TODO: class Key { std::shared_pointer<KeyImpl> } ?
    KeyContainer keys_;

    // handle GLFW scroll
    static void scroll_callback_(GLFWwindow* , double , double );
    std::vector<GLFWscrollfun> scroll_cbks_;
    std::vector<KeyMouseScroll*> scrolls_x_;
    std::vector<KeyMouseScroll*> scrolls_y_;

    // add a Key. two different Key's with same implementation can be added,
    // since they may have different settings (fex. canDisable)
    template <typename KeyT>
    KeyT* push(KeyT* k);

};




// TODO: push unique, using typeid (and then look at code/...), to prevent duplicates!
template <typename KeyT>
inline KeyT* Keys::push(KeyT* k)
{
    // look at key.type_!

    keys_.push_back( k ); 
    return k; 
} 

////////////////////////////////////////////////////////////////////////////////
//  


} // namespace keys

} // namespace batb


#endif

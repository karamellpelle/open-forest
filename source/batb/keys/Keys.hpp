//    open-forest: an orientering game.
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
#ifndef BATB_KEYS_KEYS_HPP
#define BATB_KEYS_KEYS_HPP
#include "batb/batb_include.hpp"

namespace batb
{

class BATB;


namespace keys
{

class Key;
class KeyButton;      
class KeyMouseButton; 
class KeyMouseAxisX;  
class KeyMouseAxisY;  
class KeyClicker;     
class KeyAlpha;       
class KeyPointer;     
class KeyPointer;


class Keys
{
friend void begin(Keys& keys);
friend void end(Keys& keys);

public:
    Keys(BATB& b) : batb( b )
    {
    }


    BATB& batb;
    
    void clear();
    void reset();
    void step(tick_t t);

    // get cursor pos, in pixels
    void getCursorPos(uint& x, uint& y)
    {
        // TODO: use env instead of GLFW?
        double x_, y_;
        glfwGetCursorPos( window_, &x_, &y_ );
        x = (uint)( x_ );
        y = (uint)( y_ );
    }
    // get cursor pos, relative to screen shape 
    void getCursorPos(double& x, double& y)
    {
        // TODO: use env instead of GLFW?
        int wth; int hth;
        glfwGetFramebufferSize( window_, &wth, &hth );
        float_t scale = 1.0 / (float_t)( std::max( wth, hth ) );

        double x_, y_;
        glfwGetCursorPos( window_, &x_, &y_ );
        x = (float_t)( x_ ) * scale;
        y = (float_t)( y_ ) * scale;
    }
    void setCursorFree(bool ); // default false

    int getKey(int k)
    {
        return glfwGetKey( window_, k );
    }
    int getMouseButton(int k)
    {
        return glfwGetMouseButton( window_, k );
    }

    // let this object call back 
    void charCalling(GLFWcharfun );
    void keyCalling(GLFWkeyfun );
    void mousebuttonCalling(GLFWmousebuttonfun );
    void cursorposCalling(GLFWcursorposfun );
    void scrollCalling(GLFWscrollfun );

    ////////////////////////////////////////////////////////////////////////////////
    // Key's

    // prims
    KeyButton*      createKeyButton(int code);
    KeyMouseButton* createKeyMouseButton(int code);
    KeyMouseAxisX*  createKeyMouseAxisX();
    KeyMouseAxisY*  createKeyMouseAxisY();
    // cons
    KeyClicker*     createKeyClicker(Key* k);
    KeyAlpha*       createKeyAlpha(Key* k);
    KeyPointer*     createKeyPointer(Key* x, Key* y, Key* l, Key* r);
    KeyPointer*     createKeyPointer();
    // from definition
    Key* createKey(const YAML::Node& );


private:
    bool initialized_ = false;

    GLFWwindow* window_ = nullptr;

    // previous cursor
    bool cursor_free_ = false;
    double cursor_x0_ = 0;
    double cursor_y0_ = 0;

    ////////////////////////////////////////////////////////////////////////////////
    // Key's
    //
    using KeyContainer = std::vector<Key*>; // TODO: class Key { std::shared_pointer<KeyImpl> } ?
    KeyContainer keys_;

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

void begin(Keys& keys);

void end(Keys& keys);


} // namespace keys

} // namespace batb


#endif

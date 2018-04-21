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
#ifndef BATB_KEYS_KEY_ALPHA_HPP
#define BATB_KEYS_KEY_ALPHA_HPP
#include "BATB/Keys/Key.hpp"


namespace batb
{

namespace keys
{


// build up alpha value, based on child Key in lower/upper part of.
class KeyAlpha : public Key
{
public:
    KeyAlpha(Keys* keys, Key* k) : KeyAlpha(keys, k, -1.0, 1.0 )                                                                { }
    KeyAlpha(Keys* keys, Key* k, float_t a, float_t b) : KeyAlpha( keys, k, a, b, 0.5 )                                         { }
    KeyAlpha(Keys* keys, Key* k, float_t a, float_t b, float_t c) : Key( keys ), key_( k ), a_( a ), b_( b ), clear_( c ), alpha_( c ) { }


    virtual void reset() override                 { alpha_ = clear_; }
    virtual void step(tick_t tick) override
    {
        // FIXME: prevent too big dt, espcecially first time...
        tick_t dt = tick - tick_prev_;           // assuming dt not too smal, i.e. when high frame rate and dt ~= 0...
        float_t x = key_->alpha();
        alpha_ +=  (tick_t)( dt * (tick_t)( a_ * (1.0 - x) + b_ * x ) );
        alpha_ = clamp( alpha_, (float_t)0.0, (float_t)1.0 );
        tick_prev_ = tick;
    }
    virtual float_t alpha() override              { return alpha_; }
    virtual void canDisable(bool b) override      { key_->canDisable( b ); }

    // names
    virtual std::string name()     const override; // unique identifier
    virtual std::string nameEasy() const override; // human readable
    virtual std::string nameGUI()  const override; // GUI widget


    ////////////////////////////////////////////////////////
    //    settings
    void set(float_t a, float_t b)
    {
        a_ = a;
        b_ = b;
    }
    void set_clear(float_t clear)
    {
        clear_ = clear;
    }


private:
    Key* const key_;

    tick_t tick_prev_;
    float_t a_;
    float_t b_;
    float_t clear_;
    float_t alpha_;

};

} // namespace keys

} // namespace batb

#endif

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
#ifndef BATB_KEYS_KEY_ALPHA_HPP
#define BATB_KEYS_KEY_ALPHA_HPP
#include "batb/keys/Key.hpp"


namespace batb
{

namespace keys
{


// build up alpha value, based on child Key in lower/upper part of.
class KeyAlpha : public Key
{
friend class KeySet;

public:
    virtual void clear() override                 { alpha_ = clear_; }
    virtual void update(tick_t tick) override
    {
        // FIXME: prevent too big dt, espcecially first time...
        tick_t dt = tick - tick_prev_;           // assuming dt not too smal, i.e. when high frame rate and dt ~= 0...
        float_t x = key_->alpha();
        alpha_ +=  (tick_t)( dt * (tick_t)( a_ * (1.0 - x) + b_ * x ) );
        alpha_ = keep_inside( 0.0, 1.0, alpha_ );
        tick_prev_ = tick;
    }
    virtual float_t alpha() override              { return alpha_; }


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
    KeyAlpha(Key* k) : KeyAlpha(k, -1.0, 1.0 )                                                                { }
    KeyAlpha(Key* k, float_t a, float_t b) : KeyAlpha( k, a, b, 0.5 )                                         { }
    KeyAlpha(Key* k, float_t a, float_t b, float_t c) : key_( k ), a_( a ), b_( b ), clear_( c ), alpha_( c ) { }

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

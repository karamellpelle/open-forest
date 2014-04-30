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
#ifndef REF_HPP
#define REF_HPP
#include "GameInclude.hpp"



namespace Game
{


// USAGE
// subclassing Ref for an deriving object O:
//
//  A:  if O should have the ability to be container-released,
//      implement the free-mem capability in 'void Ref::destroy_()'
//  B:  else, do not implement 'void Ref::destroy_()'
// 
//  for case B, if O is unintentionally is container-released, 
//  then this releasment will not free memory too early
//  (better with memory leak than memory corruption)

class Ref
{
public:
    
    // hold object in container
    static void hold(Ref* ref)
    {
        ++ref->count_;
    }

    // release object from container
    static void release(Ref* ref)
    {
#ifdef GAME_DEBUG_REF_OUTPUT
        std::clog << "Ref::release( " << ref << " ), count is " << ref->count_ << ". ";
#endif
        if ( ref == 0 )
        {
            // ???
        }
        else
        {
            --ref->count_; 
            if ( ref->release_ )
            {
                if ( ref->count_ == 0 )
                {
#ifdef GAME_DEBUG_REF_OUTPUT
                std::clog << "Ref::destroy_()";
#endif
                    ref->destroy_();
                }

            }
        }
#ifdef GAME_DEBUG_REF_OUTPUT
        std::clog << std::endl;
#endif

    }

    // tell object to be released
    template <typename RefA>
    static RefA* releasing(RefA* ref)
    {
        ref->release_ = true;
        return ref;
    }

    // destroy object (static interface)
    static void destroy(Ref* ref)
    {
        ref->destroy_();
    }
    
protected:
    Ref() : count_( 0 ), release_( false ) { }
    // FIXME: prevent public operator delete?

    // implement memory release here (for container-release)
    virtual void destroy_() { };

    static uint count(Ref* ref)
    {
        return ref->count_;
    }

private:
    Ref(const Ref& );             // only working with pointers to Ref 
    Ref& operator=(const Ref& );  // only working with pointers to Ref

    uint count_;                  // number of Game-references
    bool release_;                // release on down to 0?

};

}


#endif

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
#ifndef BATB_OGRE_OGRE_HPP
#define BATB_OGRE_OGRE_HPP
#include "batb/batb_include.hpp"


// forward declare Ogre classes
namespace Ogre
{
    class Root;
    class RenderWindow;
    class RenderTarget;

}


namespace batb
{

class BATB;

// tmp:
namespace tmp
{
namespace ogre
{
void demo_begin(BATB& );
void demo_iterate(BATB& );
}
}


namespace ogre
{




class OGRE
{
friend void begin(OGRE& );
friend void end(OGRE& );

// TMP:
friend void tmp::ogre::demo_begin(BATB& );
friend void tmp::ogre::demo_iterate(BATB& );

public:
    OGRE(BATB& b);

    void filepath(const std::string& path)
    {
        filepath_ = path;
    }
    void save();



    BATB& batb;

private:
    bool initialized_ = false;
    std::string filepath_;

    Ogre::Root* root_ = nullptr;
    Ogre::RenderWindow* renderwindow_ = nullptr; 
    Ogre::RenderTarget* rendertarget_ = nullptr;
};


void begin(OGRE& );
void end(OGRE& );

} // namespace ogre

} // namespace batb

#endif


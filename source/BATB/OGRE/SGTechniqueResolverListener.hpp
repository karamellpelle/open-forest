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
#ifndef BATB_OGRE_STTECHNIQUERESOLVERLISTENER_HPP
#define BATB_OGRE_STTECHNIQUERESOLVERLISTENER_HPP

#include "OgreRTShaderSystem.h"
#include "OgreMaterialManager.h"

namespace batb
{

class BATB;

namespace ogre 
{


// this is based on the OgreBites class
class SGTechniqueResolverListener : public Ogre::MaterialManager::Listener {
public:
    SGTechniqueResolverListener(BATB* );

    Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
                                          const Ogre::String& schemeName,
                                          Ogre::Material* originalMaterial, unsigned short lodIndex,
                                          const Ogre::Renderable* rend);

    bool afterIlluminationPassesCreated(Ogre::Technique* tech);

    bool beforeIlluminationPassesCleared(Ogre::Technique* tech);

protected:
    Ogre::RTShader::ShaderGenerator* mShaderGenerator; // The shader generator instance.
};

} // namespace ogre



} // namespace batb

#endif

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
#include "batb/tmp/ogre.hpp"
#include "batb.hpp"
#include "batb/ogre.hpp"

#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreEntity.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreViewport.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreWindowEventUtilities.h"

namespace batb
{


namespace tmp
{

namespace ogre
{


static bool tmp_empty = true;

static Ogre::SceneManager* scenemgr = nullptr;
static Ogre::Camera* camera = nullptr;
static Ogre::Viewport* viewport = nullptr;

void demo_begin(BATB& batb)
{
    if ( tmp_empty )
    {
        // see http://www.ogre3d.org/tikiwiki/SceneManagersFAQ for managers
        scenemgr = batb.ogre.root_->createSceneManager( "DefaultSceneManager" );
        
        camera = scenemgr->createCamera( "PlayerCam" );
        camera->setPosition( Ogre::Vector3( 0, 0, 80 ) );
        camera->lookAt( Ogre::Vector3( 0, 0, -300 ) );
        camera->setNearClipDistance( 5 );

        viewport = batb.ogre.renderwindow_->addViewport( camera );
        viewport->setBackgroundColour( Ogre::ColourValue(0, 0, 0) );

        // FIXME: into demo_iterate
        camera->setAspectRatio( Ogre::Real( viewport->getActualWidth()
                                / Ogre::Real( viewport->getActualHeight() )) );

        Ogre::Entity* head = scenemgr->createEntity( "Head", "ogrehead.mesh" );
        Ogre::SceneNode* node = scenemgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject( head );

        scenemgr->setAmbientLight( Ogre::ColourValue(0.5, 0.5, 0.5));

        Ogre::Light* l = scenemgr->createLight( "MainLight" );
        l->setPosition( 20, 80, 50 );
    }
    tmp_empty = false;
}

void demo_iterate(BATB& batb)
{
    // RenderTarget::update, Root::renderOneFrame

    Ogre::WindowEventUtilities::messagePump();

    // TODO: Root::renderOneFrame or RenderWindow::renderXXX
    batb.ogre.root_->renderOneFrame();
}





} // namespace ogre

} // namespace tmp


} // namespace batb





#include <algorithm>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#include "matrix.h"
#include "point.h"
#include "map.h"
#include "course.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"
#include "observer.h"
#include "eventproxy.h"
#include "database.h"


EventProxy::EventProxy()
{
#if 0
    map = Database::instance().getEventMap();
    course = Database::instance().getEventCourse();
    terrain = Database::instance().getEventTerrain();
#endif
#if 0
    headlights[0] = GL_LIGHT0;
    headlights[1] = GL_LIGHT1;
    headlights[2] = GL_LIGHT2;
    headlights[3] = GL_LIGHT3;
    headlights[4] = GL_LIGHT4;
    headlights[5] = GL_LIGHT5;
    headlights[6] = GL_LIGHT6;
    headlights[7] = GL_LIGHT7;
#endif
}

EventProxy::~EventProxy()
{
}

void EventProxy::realize()
{
    TerrainProxy& terrainProxy = TerrainProxy::instance();
    mesh = terrainProxy.getMesh();
}

/*
 * Draw an extremely simple sky dome for experimentation.
 * This is like a pyramid hanging over the world and we see
 * the inside of it.
 * A pyramid can be nicely drawn using OpenGL triangle fan.
 * The apex of the pyramid is deep blue color and towards 
 * the horizon the color changes to light cyan.
 * The apex is always located above the viewer.
 * This should be subdivided to make the colors smoother.
 * Use a real dome.
 * Sky dome probably would benefit from being stored in a display list.
 */
void EventProxy::drawSkyDome(Point pos)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0, 0.16, 0.9058); // dark blue 
    glVertex3f(pos.x, pos.y, 80.0);
    glColor3f(0.541, 0.6196, 1.0); // light blue 
    glVertex3f(pos.x-200.0, pos.y-200.0, -50.0);
    glVertex3f(pos.x-200.0, pos.y+200.0, -50.0);
    glVertex3f(pos.x+200.0, pos.y+200.0, -50.0);
    glVertex3f(pos.x+200.0, pos.y-200.0, -50.0);
    glVertex3f(pos.x-200.0, pos.y-200.0, -50.0);
    glEnd();
}

int EventProxy::draw(Location* location)
{
    //drawSkyDome(location->position);
    mesh->draw(location->position, location->horizontal+PI/2);

    // for remote orienteers, 
    //     draw it to terrain
    //     send the new coordinates to peer
    return observers.drawToTerrain(location);
}

int EventProxy::drawDay(Location* location)
{
    //drawSkyDome(location->position);
    mesh->drawDay(location->position, location->horizontal+PI/2);

    // for remote orienteers, 
    //     draw it to terrain
    //     send the new coordinates to peer
    return observers.drawToTerrainDay(location);
}

int EventProxy::drawNight(Location* location)
{
    //drawSkyDome(location->position);
    mesh->drawNight(location->position, location->horizontal+PI/2);

    // for remote orienteers, 
    //     draw it to terrain
    //     send the new coordinates to peer
    return observers.drawToTerrainNight(location);
}

int EventProxy::getCoordinates(GLfloat* pos, GLfloat* hor, GLfloat* ver, int index)
{
    return observers.getCoordinates(pos, hor, ver, index);
}


void EventProxy::drawToMap(Location* location)
{
    observers.drawToMap(location);
}

void EventProxy::addObserver(Observer* observer)
{
    observers.add(observer);
}

void EventProxy::removeObserver(Observer* observer)
{
    observers.remove(observer);
}

void EventProxy::removeAllObservers()
{
    observers.removeAll();
}

fptype EventProxy::getLowestPoint()
{
    return mesh->getLowestPoint();
}

fptype EventProxy::getHighestPoint()
{
    return mesh->getHighestPoint();
}


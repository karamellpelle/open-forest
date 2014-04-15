#include <vector>
#include <cmath>
#include <stdio.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "common.h"
#include "matrix.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"
#include "mapproxy.h"
#include "database.h"



Contour::~Contour()
{
    std::for_each(points.begin(), points.end(), del_fun<Point>());
}

void Contour::addHead(Point* point)
{
    int s = points.size();
    if (s > 0)
    {
	Point* p = points.front();
	fptype d = p->distance(*point);
	if (d > 30.0)
	    std::cout << "size = " << s << " distance = " << d << std::endl;
    }

    points.push_front(point); 
}

void Contour::addTail(Point* point)
{
    int s = points.size();
    if (s > 0)
    {
	Point* p = points.back();
	fptype d = p->distance(*point);
	if (d > 30.0)
	    std::cout << "size = " << s << " distance = " << d << std::endl;
    }
    points.push_back(point); 
}

void Contour::addLeft()
{
    left++;
}

void Contour::addRight()
{
    right++;
}

void Contour::setDownhill(Contour::ContourType contourType)
{
    //std::cout << "Countour: " << contourType << " left: " << left << " right: " << right << '\n';
    if (left > right)
	dir = Contour::LEFT;
    else if (left < right)
	dir = Contour::RIGHT;
    else
	dir = Contour::DONTKNOW;
}

int Contour::draw()
{
    int mult;
    int count = 45;
    switch (dir)
    {
    case LEFT: mult = 1; break;
    case RIGHT: mult = -1; break;
    default: mult = 0; break;
    }
    Point* point = NULL;
    Point* point2 = NULL;
    glBegin(GL_LINE_STRIP);
    for (Points::const_iterator i = points.begin(); i != points.end(); ++i)
    {
	point = *i;
	glVertex2f(point->x, point->y);
	if (++count%50 == 0) 
	{
	    /* draw line showing the direction of downhill */
	    fptype slope = atan2(point2->y - point->y, point2->x - point->x);
	    slope += PI/2;

	    glVertex2f(point->x + mult*cos(slope),
		       point->y + mult*sin(slope));
	    glVertex2f(point->x, point->y);
	}
	point2 = point;
    }
    glEnd();
    return 0;
}


MapProxy::~MapProxy()
{
    std::for_each(contours.begin(), contours.end(), del_fun<Contour>());
}


// draw north-south lines of the map
void MapProxy::drawNSlines()
{
    int width = map->getWidth();
    int height = map->getHeight();

    // draw north-south lines
    glColor3f(0.7, 0.7, 0.7);
    for (int i = 20; i < width; i += 50) 
    {
	glBegin(GL_LINE_STRIP);

	/* draw the line */
	glVertex2f((float)i, 0.0);
	glVertex2f((float)i, height+5);

	/* draw a triangle pointing north */
	glVertex2f((float)i-3, height);
	glVertex2f((float)i+3, height);
	glVertex2f((float)i, height+5);

	glEnd();
    }
}

void MapProxy::drawContours()
{
    std::for_each(contours.begin(), contours.end(), std::mem_fun(&Contour::draw));
}

void MapProxy::makeMapDL()
{
    if ((dliMap = glGenLists(1)) != 0) 
    {
	glNewList(dliMap, GL_COMPILE);
	drawNSlines();
	drawContours();
	glEndList();
    }
    else 
    {
	printf("mpMakeMapDL: can't generate display lists\n");
    }
}

float MapProxy::initRealize()
{
    Database& db = Database::instance();
    TerrainProxy& tp = TerrainProxy::instance();
    Mesh* mesh = tp.getMesh();
    Map* selectedMap = db.getEventMap();
    float percentComplete;

    if (map != selectedMap)
    {
	map = selectedMap;
	contourInterval = map->getContourInterval();
	int width = mesh->getWidth();
	int height = mesh->getHeight();
	map->setWidth(width);
	map->setHeight(height);
	std::for_each(contours.begin(), contours.end(), del_fun<Contour>());
	contours.clear();
	highestPoint = mesh->getHighestPoint();
	fptype low  = mesh->getLowestPoint();
	fptype c = fmod(low, contourInterval);
	lowestContourLevel = int(low-c); // lowest contour level
	altitude = lowestContourLevel;
	percentComplete = 0.0;
    }
    else
    {
	percentComplete = 1.0;
    }

    return percentComplete;
}

float MapProxy::continueRealize()
{
    TerrainProxy& tp = TerrainProxy::instance();
    Mesh* mesh = tp.getMesh();
    float percentComplete;
    int width = mesh->getXSize();
    int height = mesh->getYSize();

    if (altitude < highestPoint) 
    {
	mesh->clearTriangles();
	for (int i = 0; i < width; i++) 
	{
	    for (int j = 0; j < height; j++) 
	    {
		mesh->findContours2(contours, altitude, i, j);
    	    }
	}
	altitude += contourInterval;
    }

    percentComplete = altitude/float(highestPoint);
    return percentComplete;
}

void MapProxy::drawCircle(fptype x, fptype y, fptype radius)
{
    glBegin(GL_LINE_LOOP);
    for (fptype angle = 0; angle < 2*PI; angle += 0.2) {
	glVertex2f(x + radius*cos(angle),
		   y + radius*sin(angle));
    }
    glEnd();
}

void MapProxy::drawThumb(Point& thumb)
{
    glColor3f(0.0, 0.0, 1.0);
    drawCircle(thumb.x, thumb.y, 2);
}

void MapProxy::draw(Point& thumb)
{
    if (dliMap != 0) 
    {
	glCallList(dliMap);
    }
    else 
    {
	drawNSlines();
	drawContours();
    }
    drawThumb(thumb);
#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

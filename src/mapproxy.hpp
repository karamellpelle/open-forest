#ifndef _MAPPROXY_H
#define _MAPPROXY_H

#include <deque>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "map.h"
#include "point.h"
#include "util.h"


class Contour 
{
 public:
    enum ContourType 
    {
	CT_OPEN,      // open contour falls off the edge of the map
	CT_CLOSED,    // closed contour loops back to itself
	CT_TOO_SHORT, // contour contain too few points
	CT_ERROR      // error during contour tracing
    };
    
    // direction of downhill when viewed from head to tail of contour
    enum ContourDir
    {
	LEFT,    // downhill is to the left
	RIGHT,   // downhill is to the right
	DONTKNOW // don't know where downhill is
    }; 
    typedef std::deque<Point*> Points;

    Contour(fptype _height) : height(_height), left(0), right(0) {}
    ~Contour();

    void setDownhill(Contour::ContourType contourType);
    int draw();
    void addHead(Point* point);
    void addTail(Point* point);
    int size() const { return points.size(); }
    void joinHeadAndTail();
    void addLeft();
    void addRight();

private:
    Points     points; // list of points in this contour
    fptype     height; // height value of this contour 
    ContourDir dir;    // tells the direction of downhill 
                       // when viewed from the beginning of the contour
    int left;
    int right;
};

class MapProxy 
{
 public:
    typedef std::vector<Contour*> Contours;

    ~MapProxy();

    void drawContours();
    void drawNSlines();
    void draw(Point& thumb);
    void reset();
    void makeMapDL();
    Map* getMap() const { return map; }
    void setMap(Map* m) { map = m; }

    float initRealize();
    float continueRealize();


    // implements the singleton pattern
    static MapProxy& instance()
    {
	static MapProxy theSingleInstance;
	return theSingleInstance;
    }

 private:

    MapProxy() : map(0), dliMap(0) {}

    void drawCircle(fptype x, fptype y, fptype radius);
    void drawThumb(Point& thumb);

    Map*     map;      // map that this proxy represents
    Contours contours; // list of contours in this map
    GLuint   dliMap;   // display list id

    int contourInterval;
    fptype highestPoint;
    int altitude;
    int lowestContourLevel;
};

#endif // _MAPPROXY_H

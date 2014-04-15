#ifndef _POINT_H
#define _POINT_H

#include <string>
#include "util.h"


struct Point
{
    fptype x;
    fptype y;
    fptype z;

    Point() : x(0), y(0), z(0) {}
    Point(fptype x, fptype y, fptype z);
    fptype distance(Point& p);
    bool operator==(const Point& p) const;
    std::ostream& toXML(std::ostream& ostr, int indent) const;
    std::string genID() const;
    int _glVertex2f();
    int _glVertex3f();
};

Point operator*(Point& p1, fptype d);

std::ostream& operator<<(std::ostream& ostr, const Point& point);
std::istream& operator>>(std::istream& istr, Point& point);


/* location is position plus horizontal and vertical
 * angles (= direction of view)
 * In OpenGL the default camera position is in (0,0,0)
 * looking down the negative z-axis.
 */
struct Location
{
    Point position;     // out current position
    fptype runDirection; // direction we are running at
    fptype horizontal;   // our horizontal direction of view
    fptype vertical;     // our vertical direction of view

    Location(fptype x, fptype y, fptype z, fptype rd, fptype h, fptype v);
};

#endif // _POINT_H

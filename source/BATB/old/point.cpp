#include <iostream>
#include <ostream>
#include <istream>

#include <sstream>
#include <math.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "point.h"
#include "util.h"


Point::Point(fptype xx, fptype yy, fptype zz)
    : x(xx), y(yy), z(zz)
{
}

Point operator*(Point& p1, fptype d) 
{
    return Point(p1.x*d, p1.y*d, p1.z*d); 
}

bool Point::operator==(const Point& p) const 
{
    return (x == p.x) && (y == p.y) && (z == p.z);

    /* TODO: this may not be accurate enough 
     * may have to use something like
     * self->x - other->x < EPSILON
     * where EPSILON is some small constant
     */
}

std::ostream& Point::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');
    //ostr.setf(ios_base::fixed, ios_base::floatfield);
    ostr.setf(std::ios_base::showpoint);
    ostr.precision(8);
    ostr << i
	 << "<point>"
	 <<   "<x>" << x << "</x>"
	 <<   "<y>" << y << "</y>"
	 <<   "<z>" << z << "</z>"
	 << "</point>"
	 << std::endl;
    return ostr;
}

std::string Point::genID() const
{
    std::ostringstream ostr;
    ostr << x << y << z;
    return std::string(md5sum(ostr.str().c_str()));
}

int Point::_glVertex2f()
{
    glVertex2f(x, y);
    return 0;
}

int Point::_glVertex3f()
{
    glVertex3f(x, y, z);
    return 0;
}

std::ostream& operator<<(std::ostream& ostr, const Point& point)
{
    ostr << point.x << " " << point.y << " " << point.z;
    return ostr;
}

std::istream& operator>>(std::istream& istr, Point& point)
{
    istr >> point.x >> point.y >> point.z;
    return istr;
}

/**
 * Calculate the distance between two points.
 */
fptype Point::distance(Point& p)
{
    fptype nx = x - p.x;
    fptype ny = y - p.y;
    return sqrt(nx*nx + ny*ny);
}

Location::Location(fptype x, fptype y, fptype z, 
		   fptype rd, fptype h, fptype v)
    : position(Point(x, y, z)), runDirection(rd),
      horizontal(h), vertical(v)
{
}

std::ostream& operator<<(std::ostream& ostr, Location& location)
{
    ostr << location.position.x << " "
	 << location.position.y << " "
	 << location.position.z << " "
	 << location.runDirection << " "
	 << location.horizontal << " "
	 << location.vertical;
    return ostr;
}

std::istream& operator>>(std::istream& istr, Location& location)
{
    istr >> location.position.x
	 >> location.position.y
	 >> location.position.z
	 >> location.runDirection
	 >> location.horizontal
	 >> location.vertical;
    return istr;
}


#ifndef _CONTROL_H
#define _CONTROL_H

#include "point.h"

struct Control
{
    Point position;

    Control(fptype x, fptype y, fptype z);
    Control(Point& p);
    void draw() const;
    std::ostream& toXML(std::ostream& ostr, int indent) const;
    std::string genID() const;
    int reset();
};

#endif // _CONTROL_H

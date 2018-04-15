#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "event.h"
#include "util.h"


std::string Event::genId() const
{
    std::ostringstream ostr;

    ostr << _mapId << _courseId << _night << _fog;

    return std::string(md5sum(ostr.str().c_str()));
}

Event::Event(std::string id, std::string name, std::string mapid, 
	     std::string courseid, bool night, bool fog)
    : _id(id), _name(name), _mapId(mapid), 
      _courseId(courseid), _night(night), _fog(fog)
{
}

Event::Event(std::string name, std::string mapid, 
	     std::string courseid, bool night, bool fog)
    : _name(name), _mapId(mapid), 
      _courseId(courseid), _night(night), _fog(fog)
{
    _id = genId();
}

Event::Event(std::string mapid, std::string courseid)
    : _name("Foobar"), _mapId(mapid), 
      _courseId(courseid), _night(false), _fog(false)
{
    _id = genId();
}

Event::Event(std::string name, std::string mapid, std::string courseid)
    : _name(name), _mapId(mapid), 
      _courseId(courseid), _night(false), _fog(false)
{
    _id = genId();
}

std::ostream& Event::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');
    
    ostr << i << "<event>\n";
    ostr << i << "  <id>" << _id << "</id>\n";
    ostr << i << "  <name>" << _name << "</name>\n";
    ostr << i << "  <mapId>" << _mapId << "</mapId>\n";
    ostr << i << "  <courseId>" << _courseId << "</courseId>\n";
    ostr << i << "  <night>" << (_night ? "true" : "false") << "</night>\n";
    ostr << i << "  <fog>" << (_fog ? "true" : "false") << "</fog>\n";
    ostr << i << "</event>\n";

    return ostr;
}

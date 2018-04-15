#include <iostream>
#include <string>

#include "orienteer.h"
#include "util.h"


Orienteer::Orienteer()
{
    _name = "Markus";
    _id = md5sum(_name.c_str());
}

Orienteer::Orienteer(std::string name)
{
    _name = name;
    _id = md5sum(_name.c_str());
}

Orienteer::Orienteer(std::string name, std::string id)
{
    _name = name;
    _id = id;
}

std::ostream& Orienteer::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<orienteer>\n";
    ostr << i << "  <id>" << _id << "</id>\n";
    ostr << i << "  <name>" << _name << "</name>\n";
    ostr << i << "</orienteer>\n";

    return ostr;
}

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "map.h"
#include "util.h"


std::string Map::genId() const
{
    std::ostringstream ostr;
    ostr << _terrainId << _contourInterval;
    return std::string(md5sum(ostr.str().c_str()));
}

Map::Map(std::string terrainId, int contourInterval)
    : _name("Map"), _terrainId(terrainId), _contourInterval(contourInterval)
{
}

Map::Map(std::string id, std::string name, std::string terrainId, int contourInterval)
    : _id(id), _name(name), _terrainId(terrainId), _contourInterval(contourInterval)
{
}

Map::Map(std::string name, std::string terrainId, int contourInterval)
    : _name(name), _terrainId(terrainId), _contourInterval(contourInterval)
{
    _id = genId();
}

Map::Map(std::string terrainId)
    : _name("Map"), _terrainId(terrainId), _contourInterval(5)
{
    _id = genId();
}

bool Map::operator==(const Map& other) const
{
    return _id == other._id;
}

std::ostream& Map::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<map>\n";
    ostr << i << "  <id>" << _id << "</id>\n";
    ostr << i << "  <name>" << _name << "</name>\n";
    ostr << i << "  <terrainId>" << _terrainId << "</terrainId>\n";
    ostr << i << "  <contourInterval>" << _contourInterval << "</contourInterval>\n";
    ostr << i << "</map>\n";

    return ostr;
}

std::string Map::toString() const
{
    std::ostringstream ostr;
    ostr << _name << " " << _contourInterval;
    return ostr.str();
}


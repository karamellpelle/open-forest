#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <time.h>

#include "terrain.h"
#include "util.h"


bool Terrain::operator==(const Terrain& other) const
{
    return _id == other._id;
}

std::string Terrain::genId() const
{
    std::ostringstream ostr;

    ostr << _width 
	 << _height 
	 << _altitudeVariation
	 << _seed 
	 << _nonFractalIterations
	 << _recursionLevels
	 << _fractalDimension;

    return std::string(md5sum(ostr.str().c_str()));
}

Terrain::Terrain(std::string name, std::string id,
		 int width, int height, int seed,
		 int altitudeVariation, int nonFractalIterations,
		 int recursionLevels, float fractalDimension)
    : _name(name), _id(id), _width(width), _height(height), 
      _altitudeVariation(altitudeVariation), 
      _recursionLevels(recursionLevels),
      _nonFractalIterations(nonFractalIterations),
      _fractalDimension(fractalDimension),
      _seed(seed)
{
}

Terrain::Terrain(std::string name, int width, int height, int seed,
		 int altitudeVariation, int nonFractalIterations,
		 int recursionLevels, float fractalDimension)
    : _name(name), _id(genId()), _width(width), _height(height),
      _altitudeVariation(altitudeVariation), 
      _recursionLevels(recursionLevels),
      _nonFractalIterations(nonFractalIterations),
      _fractalDimension(fractalDimension), 
      _seed(seed)
{
}

Terrain::Terrain(std::string name, int width, int height,
		 int altitudeVariation, int nonFractalIterations,
		 int recursionLevels, float fractalDimension)
    : _name(name), _id(genId()), _width(width), _height(height),
      _altitudeVariation(altitudeVariation), 
      _recursionLevels(recursionLevels),
      _nonFractalIterations(nonFractalIterations),
      _fractalDimension(fractalDimension)
{
    int seed = time((time_t*)0);
    _seed = seed % 900000000;
    _id = genId();
}

Terrain::Terrain()
    : _name("Terrain"), _width(300), _height(300),
      _altitudeVariation(30), 
      _recursionLevels(10), _nonFractalIterations(6), _fractalDimension(0.65)
{
    int seed = time((time_t*)0);
    _seed = seed % 900000000;
    _id = genId();
}

std::ostream& Terrain::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<terrain>\n";
    ostr << i << "  <id>" << _id << "</id>\n";
    ostr << i << "  <name>" << _name << "</name>\n";
    ostr << i << "  <width>" << _width << "</width>\n";
    ostr << i << "  <height>" << _height << "</height>\n";
    ostr << i << "  <altitudeVariation>" << _altitudeVariation << "</altitudeVariation>\n";
    ostr << i << "  <seed>" << _seed << "</seed>\n";
    ostr << i << "  <nonFractalIterations>" << _nonFractalIterations << "</nonFractalIterations>\n";
    ostr << i << "  <recursionLevels>" << _recursionLevels << "</recursionLevels>\n";
    ostr << i << "  <fractalDimension>" << _fractalDimension << "</fractalDimension>\n";
    ostr << i << "</terrain>\n";

    return ostr;
}

std::string Terrain::toString() const
{
    std::ostringstream ostr;
    ostr << _name << " " 
	 << _width << "x" << _height << " "
	 << _recursionLevels << " " 
	 << _nonFractalIterations << " " 
	 << _fractalDimension;
    return ostr.str();
}

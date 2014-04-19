#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <string>

class Terrain
{
 public:
    Terrain();

    Terrain(std::string name, std::string id, int width, int height, int seed,
	    int altitudeVariation, int nonFractalIterations,
	    int recursionLevels, float fractalDimension);

    Terrain(std::string name, int width, int height, int seed,
	    int altitudeVariation, int nonfractalIterations,
	    int recursionLevels, float fractalDimension);

    Terrain(std::string name, int width, int height,
	    int altitudeVariation, int nonfractalIterations,
	    int recursionLevels, float fractalDimension);

    std::string name() const { return _name; }
    std::string id() const { return _id; }
    int width() const { return _width; }
    int height() const { return _height; }
    int altitudeVariation() const { return _altitudeVariation; }
    int recursionLevels() const { return _recursionLevels; }
    int nonFractalIterations() const { return _nonFractalIterations; }
    float fractalDimension() const { return _fractalDimension; }
    int seed() const { return _seed; }
    bool operator==(const Terrain& other) const;
    std::ostream& toXML(std::ostream& ostr, int indent) const;
    std::string toString() const;

 private:
    std::string _name;   // name of this terrain
    std::string _id;     // unique id of terain

    // the following forms the identity of a terrain
    int _width;    // width, or extent in west-east direction
    int _height;   // height, or extent in south-north direction
    int _altitudeVariation;

    // xmountains parameters
    int _recursionLevels;
    int _nonFractalIterations;
    float _fractalDimension;
    int _seed;

    std::string genId() const;

};

#endif // _TERRAIN_H

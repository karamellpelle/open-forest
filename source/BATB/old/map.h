#ifndef BATB_MAP_H
#define BATB_MAP_H

#include <string>


class Map
{
 public:
    Map(std::string terrainId);
    Map(std::string terrainId, int contourInterval);
    Map(std::string name, std::string terrainId, int contourInterval);
    Map(std::string id, std::string name, std::string terrainId, int contourInterval);

    std::string id() const { return _id; }
    std::string name() const { return _name; }
    int getWidth() const { return _width; }
    void setWidth(int w) { _width = w; }
    int getHeight() const { return _height; }
    void setHeight(int h) { _height = h; }
    std::string terrainId() const { return _terrainId; }
    int getContourInterval() const { return _contourInterval; }

    std::ostream& toXML(std::ostream& ostr, int indent) const;
    std::string toString() const;
    bool operator==(const Map& other) const;

 private:    
    std::string _id;     // id of this map
    std::string _name;   // name of this map
    int         _width;  // width of this map (pixels)
    int         _height; // height of this map

    // the following forms the identity of a map
    std::string _terrainId;       // foreign key to the terrain
    int         _contourInterval; // contour interval of this map

    std::string genId() const;

};

#endif // BATB_MAP_H

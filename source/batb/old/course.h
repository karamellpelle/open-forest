#ifndef _COURSE_H
#define _COURSE_H

#include <vector>
#include <string>

#include "control.h"


class Course
{
 public:
    Course(std::string _id, std::string _name, std::string _terrainId);
    Course(std::string _name, std::string _terrainId, int width, int height, int nControls);
    Course(std::string _terrainId, int width, int height, int nControls);
    ~Course();

    std::string genId() const;
    void addControl(Control* control);
    Control* getControl(int number);
    void draw(int nextControl) const;
    void realize(int numControls, int width, int height);
    bool operator==(const Course& other) const;
    std::ostream& toXML(std::ostream& ostr, int indent) const;
    std::string toString() const;

    std::string name() const { return _name; }
    std::string id() const { return _id; }
    std::string terrainId() const { return _terrainId; }

    fptype directionToNextControl(const int controlNumber) const;
    Point controlPosition(const int controlNumber) const;
    Control* controlAt(const int controlNumber) const;
    void reset();

 private:
    std::string           _id;        // id of this course
    std::string           _name;      // name of this course
    int _nControls;                   // number of controls
    std::string           _terrainId; // foreign key of the terrain
    std::vector<Control*> controls;   // controls of the course

};

#endif // _COURSE_H

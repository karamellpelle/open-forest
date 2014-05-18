#ifndef _ORIENTEER_H
#define _ORIENTEER_H

#include <ostream>


class Orienteer 
{
 public:
    Orienteer();
    Orienteer(std::string name);
    Orienteer(std::string name, std::string id);
    std::string id() const { return _id; }
    std::string name() const { return _name; }
    std::ostream& toXML(std::ostream& ostr, int indent) const;

 private:
    std::string _id;   // id of this orienteer
    std::string _name; // name of this orienteer

};

#endif // _ORIENTEER_H

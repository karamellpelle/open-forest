#ifndef _EVENT_H
#define _EVENT_H


#include <string>


class Event
{
 public:
    Event(std::string name, std::string mapid, std::string courseid);
    Event(std::string mapid, std::string courseid);
    Event(std::string name, std::string mapid, std::string courseid, bool night, bool fog);
    Event(std::string id, std::string name, std::string mapid, std::string courseid, bool night, bool fog);

    std::ostream& toXML(std::ostream& ostr, int indent) const;

    std::string name() const { return _name; }
    std::string id() const { return _id; }
    std::string mapId() const { return _mapId; }
    std::string courseId() const { return _courseId; }
    bool night() const { return _night; }
    bool fog() const { return _fog; }

 private:
    std::string _id;   // id of this event
    std::string _name; // name of this event

    // the following forms the identity of event
    std::string _mapId;     // foreign key of the map
    std::string _courseId;  // foreign key of the course
    bool        _night;     // is it night?
    bool        _fog;       // is there fog?
    std::string genId() const;
};

#endif // _EVENT_H

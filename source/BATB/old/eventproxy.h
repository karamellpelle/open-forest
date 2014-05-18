#ifndef _EVENTPROXY_H
#define _EVENTPROXY_H


class EventProxy
{
 public:

    // implements the singleton pattern
    static EventProxy& instance()
    {
	static EventProxy theSingleInstance;
	return theSingleInstance;
    }
    virtual ~EventProxy();

    void realize();

    // pos - write positions of each orienteer into this
    // hor - write horizontal angle of each orienteer
    // var - write vertical angle of each orienteer
    // return the index of next location to write
    int draw(Location* location);
    int drawDay(Location* location);
    int drawNight(Location* location);

    void drawToMap(Location* location);
    int getCoordinates(GLfloat* pos, GLfloat* hor, GLfloat* ver, int index);

    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void removeAllObservers();

    fptype getLowestPoint();
    fptype getHighestPoint();

 private:
    Mesh* mesh;
    Observable observers;

    void drawSkyDome(Point pos);

    EventProxy();
};

#endif // _EVENTPROXY_H

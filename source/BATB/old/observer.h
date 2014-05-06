#ifndef _OBSERVER_H
#define _OBSERVER_H

/*
 * Observer manager is meant to aid in implementing 
 * observer-observable, or publish-subscribe patterns.
 * Note that it distinguishes observers by the function,
 * thus it does not work when you want to call 
 * same function for several different objects (userData).
 */

#include <list>

#include "point.h"


class Observer
{
 public:
    virtual ~Observer() {}
    virtual int drawToTerrain(Location* location) = 0;
    virtual int drawToTerrainDay(Location* location) = 0;
    virtual int drawToTerrainNight(Location* location) = 0;
    virtual void getCoordinates(GLfloat* pos, GLfloat* hor, GLfloat* ver, int index) = 0;
    virtual int drawToMap(Location* location) = 0;
};

class Observable
{
 private:
    typedef std::list<Observer*> Observers;
    Observers observers;

 public:
    Observable() {}
    ~Observable() {}

    void add(Observer* observer)
    {
	observers.push_back(observer);
    }

    void remove(Observer* observer)
    {
	observers.remove(observer);
    }

    void removeAll()
    {
	observers.erase(observers.begin(), observers.end());
    }

    int drawToTerrain(Location* location)
    {
	std::for_each(observers.begin(), observers.end(), 
		      std::bind2nd(std::mem_fun(&Observer::drawToTerrain), location));
	return 0;
    }

    int drawToTerrainDay(Location* location)
    {
	std::for_each(observers.begin(), observers.end(), 
		      std::bind2nd(std::mem_fun(&Observer::drawToTerrainDay), location));
	return 0;
    }

    int drawToTerrainNight(Location* location)
    {
	std::for_each(observers.begin(), observers.end(), 
		      std::bind2nd(std::mem_fun(&Observer::drawToTerrainNight), location));
	return 0;
    }

    int getCoordinates(GLfloat* pos, GLfloat* hor, GLfloat* ver, int index)
    {
		int i = index;
	    for (Observers::iterator oi = observers.begin(); oi != observers.end(); ++oi)
	    {
			Observer* observer = *oi;
			observer->getCoordinates(pos, hor, ver, i++);
	    }
		return i;
    }

	void drawToMap(Location* location)
    {
	std::for_each(observers.begin(), observers.end(), 
		      std::bind2nd(std::mem_fun(&Observer::drawToMap), location));
    }
};

#endif // _OBSERVER_H

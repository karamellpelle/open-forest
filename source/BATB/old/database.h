#ifndef _DATABASE_H
#define _DATABASE_H

#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <exception>

#include "terrain.h"
#include "map.h"
#include "course.h"
#include "run.h"
#include "orienteer.h"
#include "event.h"


/*
 *
 * Data model:
 *
 * +---------+   +--------+   +---------+
 * |         |<--| course |<--|         |
 * |         |   +--------+   |         |   +-------+   +---------+
 * | terrain |                |  event  |<--|  run  |-->|orienteer|
 * |         |   +--------+   |         |   +-------+   +---------+
 * |         |<--|  map   |<--|         |
 * +---------+   +--------+   +---------+
 *
 * terrain is a standalone object (no references to other objects)
 * map is drawn of a particular terrain (refers to the terrain)
 * course is set in a particular terrain (refers to the terrain)
 * event is of particular course run on a particular map (refers to map and course)
 * run is done on a particular event by a particular orienteer (refers to event and orienteer)
 * orienteer is a standalone object (no references to other objects)
 *
 * Deletes are cascaded:
 * If a terrain is deleted, all associated courses and maps are deleted,
 * AND all events associated to deleted map or course,
 * AND all runs associated to deleted event
 *
 * If orienteer is deleted, all associated runs are deleted.
 *
 */

#ifdef WIN32
#pragma warning( disable : 4290 )
#pragma warning( disable : 4996 )
#endif

class DbException : public std::exception
{
 protected:
    std::string message;
 public:
    DbException(const std::string& msg) : message(msg) {}
    virtual std::string getMessage() const { return message; }
    virtual ~DbException() throw() {}
};

class Exists : public DbException
{
 public:
    Exists(const std::string& msg) : DbException(msg) {}
};

class NotFound : public DbException
{
 public:
    NotFound(const std::string& msg) : DbException(msg) {}
};


class ServerInfo
{
public:
    ServerInfo(std::string hostname, time_t lastUsed)
	: _hostname(hostname), _lastUsed(lastUsed) {}
    std::string hostname() const { return _hostname; }
    time_t lastUsed() const { return _lastUsed; }
    void lastUsed(time_t lu) { _lastUsed = lu; }
    std::ostream& toXML(std::ostream& ostr, int indent) const;
    bool operator<(const ServerInfo& other) {return _lastUsed > other._lastUsed; }

private:
    std::string _hostname;
    time_t _lastUsed;
};

class DatabaseObserver
{
 public:
    virtual ~DatabaseObserver() {}
    virtual int updateTerrains(void* callData) const { return 0; }
    virtual int updateMaps(void* callData) const { return 0; }
    virtual int updateCourses(void* callData) const { return 0; }
    virtual int updateRuns(void* callData) const { return 0; }
    virtual int updateEvents(void* callData) const { return 0; }
    virtual int updateOrienteers(void* callData) const { return 0; }
    virtual int updateServers(void* callData) const { return 0; }

    struct terrainsUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const 
	{
	    return observer->updateTerrains(callData);
	}
    };

    struct mapsUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const
	{
	    return observer->updateMaps(callData);
	}
    };

    struct coursesUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const
	{
	    return observer->updateCourses(callData);
	}
    };

    struct runsUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const
	{
	    return observer->updateRuns(callData);
	}
    };

    struct eventsUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const
	{
	    return observer->updateEvents(callData);
	}
    };

    struct orienteersUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const
	{
	    return observer->updateOrienteers(callData);
	}
    };

    struct serversUpdate : public std::binary_function<DatabaseObserver*, void*, int>
    {
	int operator()(const DatabaseObserver* observer, void* callData) const
	{
	    return observer->updateServers(callData);
	}
    };
};


class DatabaseObject
{
 private:
    std::string id;
 public:
    std::ostream& toXML(std::ostream& ostr, int indent);
};


class Database 
{
 public:
    typedef std::vector<Terrain*> Terrains;
    typedef std::vector<Map*> Maps;
    typedef std::vector<Course*> Courses;
    typedef std::vector<Run*> Runs;
    typedef std::vector<Orienteer*> Orienteers;
    typedef std::vector<Event*> Events;
    typedef std::vector<ServerInfo*> Servers; // TODO: use priority queue for servers???
    typedef std::vector<DatabaseObserver*> Observers;

    void addObserver(DatabaseObserver* observer);
    void removeObserver(DatabaseObserver* observer);

    void updateTerrainObservers(void* callData) const;
    void updateMapObservers(void* callData) const;
    void updateCourseObservers(void* callData) const;
    void updateRunObservers(void* callData) const;
    void updateOrienteerObservers(void* callData) const;
    void updateEventObservers(void* callData) const;
    void updateServerObservers(void* callData) const;

    void discard();
    bool write(const std::string& filename);


 private:
    void quick_create(std::string name, int width, int height,
		      int altitudeVariation, int nonFractalIterations,
		      int recursionLevels, float fractalDimension,
		      int contourInterval, int numControls);
 public:
    void ensureMinimalDB();
    void quick_create_short();
    void quick_create_medium();
    void quick_create_long();


    void addMPEvent(Terrain* terrain, Map* map, Course* course, Event* event);


    std::string getUniqueTerrainName(std::string base);

    void addTerrain(Terrain* terrain) throw(Exists);
    // PURPOSE: Add a new terrain to the database
    // REQUIRE: exists does not find the terrain
    // PROMISE: The terrain will be added to the database

    void removeTerrain(Terrain* terrain) throw(NotFound);
    // PURPOSE: Remove the terrain from database
    // REQUIRE: Terrain must be in the database (exists must return true)
    // PROMISE: Terrain is removed from the database (exists returns false)

    bool exists(Terrain* terrain) const throw();
    // PURPOSE: See if the terrain exists in database
    // REQUIRE: Nothing
    // PROMISE: Return value is true if the terrain is in database, false otherwise

    Terrain* getTerrainByName(const std::string& name) const throw(NotFound);
    // PURPOSE: Get the terrain by name
    // REQUIRE: Terrain with this name exists in the database
    // PROMISE: Return the terrain with this name

    Terrain* getTerrainById(const std::string& id) const throw(NotFound);
    // PURPOSE: Return the terrain that has the given id
    // REQUIRE: The terrain with this id exists in the database
    // PROMISE: Return the terrain with this id

    void setSelectedTerrain(Terrain* terrain) throw(NotFound);
    // PURPOSE: Set the currently selected terrain
    // REQUIRE: The terrain exists in the database
    // PROMISE: getSelectedTerrain returns this terrain

    Terrain* getSelectedTerrain() const throw(NotFound);
    // PURPOSE: Get the currently selected terrain
    // REQUIRE: Nothing
    // PROMISE: The currently selected terrain is returned
    // PROMISE: If no terrain is selected, NotFound is thrown

    void setSelectedTerrainIndex(int index) throw();
    // PURPOSE: Set the index of the currently selected terrain, 
    // PURPOSE: -1 means no terrain is selected
    // REQUIRE: Nothing
    // PROMISE: getSelectedTerrainIndex returns the index

    int getSelectedTerrainIndex() const throw();
    // PURPOSE: Get the currently selected terrain index
    // REQUIRE: Nothing
    // PROMISE: The return value is the index of the currently selected terrain
    // PROMISE: return value -1 means no terrain is selected

    Terrains getTerrains() const throw();
    char** getSelectedTerrains() const throw();
    Terrain* getRandomTerrain() const throw(NotFound);
    Terrain* getEventTerrain() const throw(NotFound);
    void adjustSelectedTerrain();
    void updateTerrainsView();


    std::string getUniqueMapName(std::string base);

    void addMap(Map* map) throw(Exists);
    // PURPOSE: Add a new map to the database
    // REQUIRE: exists does not find the terrain
    // PROMISE: The terrain will be added to the database

    void removeMap(Map* map) throw(NotFound);
    // PURPOSE: Remove the map from database
    // REQUIRE: Map must be in the database (exists must return true)
    // PROMISE: Map is removed from the database (exists returns false)

    bool exists(Map* map) const throw();
    // PURPOSE: See if the map exists in database
    // REQUIRE: Nothing
    // PROMISE: Return value is true if the map is in database, false otherwise

    Map* getMapByName(const std::string& name)  const throw(NotFound);
    // PURPOSE: Get the map by name
    // REQUIRE: A map with this name exists in the database
    // PROMISE: Return value is the map with this name

    Map* getMapById(const std::string& id) const throw(NotFound);
    // PURPOSE: Return the map that has the given id
    // REQUIRE: A map with this id exists in the database
    // PROMISE: Return value is the map with this id

    Map* getMapByTerrainId(const std::string& terrainId) const throw(NotFound);
    // PURPOSE: Return the map that has the given terrain id
    // REQUIRE: A map with this terrain id exists in the database
    // PROMISE: Return value is the map with this terrain id

    void setSelectedMap(Map* map) throw(NotFound);
    // PURPOSE: Set the currently selected map
    // REQUIRE: The map exists in the database
    // PROMISE: getSelectedMap returns this map

    Map* getSelectedMap() const throw(NotFound);
    // PURPOSE: Get the currently selected map
    // REQUIRE: Nothing
    // PROMISE: The currently selected map is returned
    // PROMISE: If no map is selected, NotFound is thrown

    void setSelectedMapIndex(int index) throw();
    // PURPOSE: Set the index of the currently selected map
    // PURPOSE: -1 means no map is selected
    // REQUIRE: Nothing
    // PROMISE: getSelectedMapIndex returns the index

    int getSelectedMapIndex() const throw();
    // PURPOSE: Get the currently selected map index
    // REQUIRE: Nothing
    // PROMISE: The return value is the index of the currently selected map
    // PROMISE: return value -1 means no map is selected

    Maps getMaps() const;
    char** getSelectedMaps() const;

    Map* getRandomMap() const throw(NotFound);
    // PURPOSE: Get a randomly selected map
    // REQUIRE: There is at least one map in the database
    // PROMISE: Nothing

    Map* getEventMap() const throw(NotFound);
    // PURPOSE: Get the map of the currently selected event
    // REQUIRE: There is a currently selected event
    // PROMISE: The returned value has event id of the currently selected event

    void adjustSelectedMap();
    void updateMapsView();


    std::string getUniqueCourseName(std::string base);

    void addCourse(Course* course) throw(Exists);
    // PURPOSE: Add a new course to the database
    // REQUIRE: exists does not find the course
    // PROMISE: The course will be added to the database

    void removeCourse(Course* course) throw(NotFound);

    bool exists(Course* course) const throw();

    Course* getCourseByName(const std::string& name) const throw(NotFound);
    Course* getCourseById(const std::string& id) const throw(NotFound);
    void setSelectedCourse(Course* course) throw(NotFound);
    Course* getSelectedCourse() const throw(NotFound);
    void setSelectedCourseIndex(int index) throw();
    int getSelectedCourseIndex() const throw();
    Courses getCourses() const throw();
    char** getSelectedCourses() const throw();
    Course* getRandomCourse() const throw(NotFound);
    Course* getEventCourse() const throw(NotFound);
    void adjustSelectedCourse();
    void updateCoursesView();

    void addRun(Run* run) throw(Exists);
    void removeRun(Run* run) throw(NotFound);
    bool exists(Run* run) const throw();
    Run* getRunByName(const std::string& name) const throw(NotFound);
    Run* getRunById(const std::string& id) const throw(NotFound);
    Run* getRunByPosition(int position) const throw(NotFound);
    void setSelectedRun(Run* run) throw(NotFound);
    Run* getSelectedRun() const throw(NotFound);
    void setSelectedRunIndex(int index) throw();
    int getSelectedRunIndex() const throw();
    Runs getRuns() const throw();
    Runs getSelectedRuns2() const throw();
    char** getSelectedRuns() const throw();
    Run* getRandomRun() const throw(NotFound);
    void adjustSelectedRun();
    void updateRunsView();

    void addOrienteer(Orienteer* orienteer) throw(Exists);
    void removeOrienteer(Orienteer* orienteer) throw(NotFound);
    bool exists(Orienteer* orienteer) const throw();
    Orienteer* getOrienteerByName(const std::string& name) const throw(NotFound);
    Orienteer* getOrienteerById(const std::string& id) const throw(NotFound);
    void setSelectedOrienteer(Orienteer* orienteer) throw(NotFound);
    Orienteer* getSelectedOrienteer() const throw(NotFound);
    void setSelectedOrienteerIndex(int index) throw();
    int getSelectedOrienteerIndex() const throw();
    Orienteers getOrienteers() const throw();
    char** getSelectedOrienteers() const throw();
    Orienteer* getRandomOrienteer() const throw(NotFound);
    void adjustSelectedOrienteer();
    void updateOrienteersView();

    std::string getUniqueEventName(std::string base);
    void addEvent(Event* event) throw(Exists);
    bool exists(Event* event) const throw();
    void removeEvent(Event* event) throw(NotFound);
    Event* getEventByName(const std::string& name) const throw(NotFound);
    Event* getEventById(const std::string& id) const throw(NotFound);
    Event* getEventByCourseId(const std::string& courseId) const throw(NotFound);
    Event* getEventByMapId(const std::string& mapId) const throw(NotFound);
    void setSelectedEvent(Event* event) throw(NotFound);
    void setSelectedEventIndex(int index) throw(NotFound);
    Event* getSelectedEvent() const throw(NotFound);
    int getSelectedEventIndex() const throw();
    Events getEvents() const throw();
    char** getSelectedEvents() const throw();
    Event* getRandomEvent() const throw(NotFound);
    void adjustSelectedEvent();
    void updateEventsView();

    void addServer(const std::string& hostname);
    void addServer(ServerInfo* serverInfo);
    ServerInfo* geServerByHostname(const std::string& hostname) const;
    char** getServers() const;
    void updateServersView();
    void freeServers();

    // implements the singleton pattern
    static Database& instance()
    {
	static Database theSingleInstance;
	return theSingleInstance;
    }

 private:

    Database();

    std::ostream& writeTerrains(std::ostream& ostr, int indent) const;
    std::ostream& writeMaps(std::ostream& ostr, int indent) const;
    std::ostream& writeCourses(std::ostream& ostr, int indent) const;
    std::ostream& writeRuns(std::ostream& ostr, int indent) const;
    std::ostream& writeOrienteers(std::ostream& ostr, int indent) const;
    std::ostream& writeEvents(std::ostream& ostr, int indent) const;
    std::ostream& writeSelectedOrienteer(std::ostream& ostr, int indent) const;
    std::ostream& writeServers(std::ostream& ostr, int indent) const;
    std::ostream& writeProperties(std::ostream& ostr, int indent) const;

    void removeMapsByTerrain(Terrain* terrain);
    void removeEventsByMap(Map* map);

    void removeCoursesByTerrain(Terrain* terrain);
    void removeEventsByCourse(Course* course);

    void removeRunsByEvent(Event* event);
    void removeRunsByOrienteer(Orienteer* orienteer);

    void freeSelectedTerrains();
    void freeSelectedMaps();
    void freeSelectedCourses();
    void freeSelectedOrienteers();
    void freeSelectedEvents();
    void freeSelectedRuns();

    Observers       observers;

    Terrains        allTerrains;      // all terrains
    char**          terrains;         // the gui wants them in this format
    int             selectedTerrainsSize;
    int             selectedTerrainIndex;

    Maps            allMaps;          // all maps
    Maps            selectedMaps;     // maps where terrainId == selected terrain id
    char**          maps;             // maps where terrainId == selected terrain id
    int             selectedMapsSize;
    int             selectedMapIndex;

    Courses         allCourses;       // all courses
    Courses         selectedCourses;  // courses where terrainId == selected terrain id
    char**          courses;          // courses where terrainId == selected terrain id
    int             selectedCoursesSize;
    int             selectedCourseIndex;

    Runs            allRuns;          // all runs
    Runs            selectedRuns;     // runs where eventId == selected event id
    char**          runs;             // runs where eventId == selected event id
    int             selectedRunsSize;
    int             selectedRunIndex;

    Orienteers      allOrienteers;     // all orienteers
    char**          orienteers;
    int             selectedOrienteersSize;
    int             selectedOrienteerIndex;

    Events          allEvents;        // all events
    char**          events;
    int             selectedEventsSize;
    int             selectedEventIndex;

    bool            needSave;         // has db been changed since last save?

    Servers         mruServers;       // most recently used servers in multiplayer
    char**          servers;
    int             serversSize;
};

#endif // _DATABASE_H

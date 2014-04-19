#include <time.h>
#include <string.h>

#include <sstream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "database.h"
#include "util.h"



std::ostream& ServerInfo::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<server>\n";
    ostr << i << "  <hostname>" << _hostname << "</hostname>\n";
    ostr << i << "  <lastUsed>" << _lastUsed << "</lastUsed>\n";
    ostr << i << "</server>\n";

    return ostr;
}

Database::Database()
{
    terrains = 0;
    selectedTerrainsSize = 0;
    selectedTerrainIndex = -1;

    maps = 0;
    selectedMapsSize = 0;
    selectedMapIndex = -1;

    courses = 0;
    selectedCoursesSize = 0;
    selectedCourseIndex = -1;

    runs = 0;
    selectedRunsSize = 0;
    selectedRunIndex = -1;

    orienteers = 0;
    selectedOrienteersSize = 0;
    selectedOrienteerIndex = -1;

    events = 0;
    selectedEventsSize = 0;
    selectedEventIndex = -1;
}

void Database::discard()
{
    if (terrains != 0)
	freeSelectedTerrains();
    if (maps != 0)
	freeSelectedMaps();
    if (courses != 0)
	freeSelectedCourses();
    if (orienteers != 0)
	freeSelectedOrienteers();
    if (events != 0)
	freeSelectedEvents();
    if (runs != 0)
	freeSelectedRuns();
    if (servers != 0)
	freeServers();

    std::for_each(allTerrains.begin(), allTerrains.end(), del_fun<Terrain>());
    std::for_each(allMaps.begin(), allMaps.end(), del_fun<Map>());
    std::for_each(allCourses.begin(), allCourses.end(), del_fun<Course>());
    std::for_each(allRuns.begin(), allRuns.end(), del_fun<Run>());
    std::for_each(allOrienteers.begin(), allOrienteers.end(), del_fun<Orienteer>());
    std::for_each(allEvents.begin(), allEvents.end(), del_fun<Event>());
    std::for_each(mruServers.begin(), mruServers.end(), del_fun<ServerInfo>());
}

void Database::quick_create(std::string base, int width, int height,
			    int altitudeVariation, int nonFractalIterations,
			    int recursionLevels, float fractalDimension,
			    int contourInterval, int numControls)
{
    std::string terrainName = getUniqueTerrainName("Terrain");
    Terrain* terrain = new Terrain(terrainName, width, height, 
				   altitudeVariation, nonFractalIterations,
				   recursionLevels, fractalDimension);
    addTerrain(terrain);

    std::string mapName = getUniqueMapName("Map");
    Map* map = new Map(mapName, terrain->id(), contourInterval);
    addMap(map);

    std::string courseName = getUniqueCourseName("Course");
    Course* course = new Course(courseName, terrain->id(), terrain->width(), terrain->height(), numControls);
    addCourse(course);

    std::string eventName = getUniqueEventName(base);
    Event* event = new Event(eventName, map->id(), course->id());
    addEvent(event);
    updateEventsView();
    setSelectedEvent(event);
    updateEventObservers(0);
}

void Database::quick_create_short()
{
    std::string name("Short");
    int width = 200;
    int height = 200;
    int altitudeVariation = 40;
    int nonFractalIterations = 5;
    int recursionLevels = 10;
    float fractalDimension = 0.65;
    int contourInterval = 3;
    int numControls = 6;
    quick_create(name, width, height,
		 altitudeVariation, nonFractalIterations,
		 recursionLevels, fractalDimension,
		 contourInterval, numControls);
}

void Database::quick_create_medium()
{
    std::string name("Medium");
    int width = 400;
    int height = 400;
    int altitudeVariation = 40;
    int nonFractalIterations = 5;
    int recursionLevels = 10;
    float fractalDimension = 0.5;
    int contourInterval = 3;
    int numControls = 8;
    quick_create(name, width, height,
		 altitudeVariation, nonFractalIterations,
		 recursionLevels, fractalDimension,
		 contourInterval, numControls);
}

void Database::quick_create_long()
{
    std::string name("Long");
    int width = 600;
    int height = 600;
    int altitudeVariation = 40;
    int nonFractalIterations = 6;
    int recursionLevels = 10;
    float fractalDimension = 0.65;
    int contourInterval = 3;
    int numControls = 10;
    quick_create(name, width, height,
		 altitudeVariation, nonFractalIterations,
		 recursionLevels, fractalDimension,
		 contourInterval, numControls);
}

// make sure we have a minimum database
// i.e. at least one terrain, map, course, event, and orienteer
void Database::ensureMinimalDB()
{
    try
    {
	getSelectedEvent();
    }
    catch (const NotFound&)
    {
	quick_create_medium();
	quick_create_long();
	quick_create_short();
    }

    try
    {
	getSelectedOrienteer();
    }
    catch (const NotFound&)
    {
        Orienteer* orienteer = new Orienteer("Orienteer");
        addOrienteer(orienteer);
	updateOrienteersView();
	setSelectedOrienteer(orienteer);
	updateOrienteerObservers(0);
    }
}

std::ostream& Database::writeTerrains(std::ostream& ostr, int indent) const 
{
    std::string i(indent, ' ');

    ostr << i << "<terrains>\n";
    std::for_each(allTerrains.begin(), allTerrains.end(), printXML<Terrain*>(ostr, indent+2));
    ostr << i << "</terrains>\n";

    return ostr;
}

std::ostream& Database::writeMaps(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<maps>\n";
    std::for_each(allMaps.begin(), allMaps.end(), printXML<Map*>(ostr, indent+2));
    ostr << i << "</maps>\n";

    return ostr;
}

std::ostream& Database::writeCourses(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<courses>\n";
    std::for_each(allCourses.begin(), allCourses.end(), printXML<Course*>(ostr, indent+2));
    ostr << i << "</courses>\n";

    return ostr;
}

std::ostream& Database::writeRuns(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<runs>\n";
    std::for_each(allRuns.begin(), allRuns.end(), printXML<Run*>(ostr, indent+2));
    ostr << i << "</runs>\n";

    return ostr;
}

std::ostream& Database::writeOrienteers(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<orienteers>\n";
    std::for_each(allOrienteers.begin(), allOrienteers.end(), printXML<Orienteer*>(ostr, indent+2));
    writeSelectedOrienteer(ostr, indent+2);
    ostr << i << "</orienteers>\n";

    return ostr;
}

std::ostream& Database::writeEvents(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<events>\n";
    std::for_each(allEvents.begin(), allEvents.end(), printXML<Event*>(ostr, indent+2));
    ostr << i << "</events>\n";

    return ostr;
}

bool Database::write(const std::string& filename)
{
    std::ofstream ostr(filename.c_str(), std::ios::out | std::ios::binary);
    int indent = 0;
    std::string i(indent, ' ');

    ostr << i << "<database>\n";

    writeTerrains(ostr, indent+2);
    writeMaps(ostr, indent+2);
    writeCourses(ostr, indent+2);
    writeEvents(ostr, indent+2);
    writeOrienteers(ostr, indent+2);
    writeRuns(ostr, indent+2);
    writeServers(ostr, indent+2);

    ostr << i << "</database>\n";
    
    ostr.close();

    needSave = false;
    return true;
}

std::ostream& Database::writeSelectedOrienteer(std::ostream& ostr, int indent) const
{
    try
    {
	Orienteer* orienteer = getSelectedOrienteer();
	std::string i(indent, ' ');
        ostr << i << "<selectedOrienteer>\n";
        orienteer->toXML(ostr, indent+2);
        ostr << i << "</selectedOrienteer>\n";
    }
    catch (const NotFound&)
    {
    }

    return ostr;
}

std::ostream& Database::writeServers(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<servers>\n";
    std::for_each(mruServers.begin(), mruServers.end(), printXML<ServerInfo*>(ostr, indent+2));
    ostr << i << "</servers>\n";

    return ostr;
}

std::ostream& Database::writeProperties(std::ostream& ostr, int indent) const
{
    //std::ofstream ostr(filename.c_str(), std::ios::out | std::ios::binary);
    std::string i(indent, ' ');

    ostr << i << "<properties>\n";
    writeSelectedOrienteer(ostr, indent+2);
    writeServers(ostr, indent+2);
    ostr << i << "</properties>\n";
    
    return ostr;
}

/**************/

void Database::addObserver(DatabaseObserver* observer) 
{ 
    observers.push_back(observer);
}

void Database::removeObserver(DatabaseObserver* observer) 
{
    Observers::iterator oi = std::remove(observers.begin(), observers.end(), observer);
    observers.erase(oi, observers.end());
}

void Database::updateTerrainObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::terrainsUpdate(), callData));
}

void Database::adjustSelectedTerrain()
{
    int numTerrains = allTerrains.size();
    int selectedIndex = getSelectedTerrainIndex();

    if (numTerrains == 0)
	selectedIndex = -1;
    else if (numTerrains == 1)
	selectedIndex = 0;
    else if (selectedIndex >= numTerrains)
	selectedIndex = numTerrains - 1;
    setSelectedTerrainIndex(selectedIndex);
}

std::string Database::getUniqueTerrainName(std::string base)
{
    std::string name;
    int i = 1;
    while (true)
    {
        std::ostringstream str;
        str << base << i;
        name = str.str();
        try 
        {
            getTerrainByName(name);
            ++i;
            str.clear();
        }
        catch (const NotFound&)
        {
            break;
        }
    }
    return name;
}

void Database::addTerrain(Terrain* terrain) throw(Exists)
{
    if (exists(terrain))
        throw Exists("Terrain exists in database");

    allTerrains.push_back(terrain);
    needSave = true;
}

void Database::removeTerrain(Terrain* terrain) throw(NotFound)
{
    if (!exists(terrain))
	throw NotFound("Terrain does not exist in database");

    // cascade the delete to maps and courses
    removeMapsByTerrain(terrain);
    updateMapsView();
    adjustSelectedMap();
    updateMapObservers(0);

    removeCoursesByTerrain(terrain);
    updateCoursesView();
    adjustSelectedCourse();
    updateCourseObservers(0);

    updateEventsView();
    adjustSelectedEvent();
    updateEventObservers(0);

    // remove this terrain
    Terrains::iterator ti = std::remove(allTerrains.begin(), allTerrains.end(), terrain);
    allTerrains.erase(ti, allTerrains.end());

    needSave = true;
}

bool Database::exists(Terrain* terrain) const throw()
{
    try
    {
	getTerrainById(terrain->id());
	return true;
    }
    catch (const NotFound&)
    {
	return false;
    }
}

Terrain* Database::getTerrainByName(const std::string& name) const throw(NotFound)
{
    Terrains::const_iterator i = std::find_if(allTerrains.begin(), allTerrains.end(), 
					      compare_fun(&Terrain::name, name));
    if (i != allTerrains.end())
	return *i;
    else
	throw NotFound("Terrain not found");
}

Terrain* Database::getTerrainById(const std::string& id) const throw(NotFound)
{
    Terrains::const_iterator i = std::find_if(allTerrains.begin(), allTerrains.end(), 
					      compare_fun(&Terrain::id, id));
    if (i != allTerrains.end())
	return *i;
    else
	throw NotFound("Terrain not found");
}

void Database::setSelectedTerrain(Terrain* terrain) throw(NotFound)
{
    Terrains::const_iterator i = std::find(allTerrains.begin(), allTerrains.end(), terrain);
    int index = (i != allTerrains.end()) ? i - allTerrains.begin() : -1;
    if (index == -1)
	throw NotFound("Terrain not found");
    setSelectedTerrainIndex(index);
}

Terrain* Database::getSelectedTerrain() const throw(NotFound)
{
    try 
    {
	return allTerrains.at(selectedTerrainIndex);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("No terrain selected");
    }
}

void Database::setSelectedTerrainIndex(int index) throw()
{
    selectedTerrainIndex = index;

    updateMapsView();
    adjustSelectedMap();
    updateMapObservers(0);

    updateCoursesView();
    adjustSelectedCourse();
    updateCourseObservers(0);
}

int Database::getSelectedTerrainIndex() const throw()
{
    return selectedTerrainIndex;
}

void Database::freeSelectedTerrains()
{
    for (int i = 0; i < selectedTerrainsSize; i++)
	delete[] terrains[i];

    delete[] terrains;
    selectedTerrainsSize = 0;
    terrains = 0;
}

// the pui puListBox widget requires the shown items as C char arrays
void Database::updateTerrainsView()
{
    if (terrains != 0)
	freeSelectedTerrains();

    terrains = new char*[allTerrains.size() + 1];
    selectedTerrainsSize = 0;
    for (Terrains::const_iterator i = allTerrains.begin(); 
	 i != allTerrains.end(); i++)
    {
	Terrain* terrain = *i;
	//std::string name = terrain->name();
	std::string name = terrain->toString();
	const char* terrainName = name.c_str();
	terrains[selectedTerrainsSize] = new char[strlen(terrainName)+1];
	strcpy(terrains[selectedTerrainsSize++], terrainName);
    }
    terrains[selectedTerrainsSize] = 0;
}

char** Database::getSelectedTerrains() const throw()
{
    return terrains;
}

Terrain* Database::getRandomTerrain() const throw(NotFound)
{
    Terrains::const_iterator iter = allTerrains.begin();
    if (iter != allTerrains.end())
	return *iter;
    else
	throw NotFound("Terrain not found");
}

// return terrain of the selected event
Terrain* Database::getEventTerrain() const throw(NotFound)
{
    Map* map = getEventMap();
    return getTerrainById(map->terrainId());
}


/**************/

void Database::updateMapObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::mapsUpdate(), callData));
}

void Database::adjustSelectedMap()
{
    int numMaps = selectedMapsSize;
    int selectedIndex = getSelectedMapIndex();

    if (numMaps == 0)
	selectedIndex = -1;
    else if (numMaps == 1)
	selectedIndex = 0;
    else if (selectedIndex >= numMaps)
	selectedIndex = numMaps - 1;
    setSelectedMapIndex(selectedIndex);
}

std::string Database::getUniqueMapName(std::string base)
{
    std::string name;
    int i = 1;
    while (true)
    {
        std::ostringstream str;
        str << base << i;
        name = str.str();
        try 
        {
            getMapByName(name);
            ++i;
            str.clear();
        }
        catch (const NotFound&)
        {
            break;
        }
    }
    return name;
}

void Database::addMap(Map* map) throw(Exists)
{
    if (exists(map))
	throw Exists("Map exists in database");

    allMaps.push_back(map);
    needSave = true;
}

void Database::removeMap(Map* map) throw(NotFound)
{
    if (!exists(map))
	throw NotFound("Map does not exist in database");

    // cascade the delete to events
    removeEventsByMap(map);
    updateEventsView();
    adjustSelectedEvent();
    updateEventObservers(0);

    Maps::iterator i = std::remove(allMaps.begin(), allMaps.end(), map);
    allMaps.erase(i, allMaps.end());

    needSave = true;
}

bool Database::exists(Map* map) const throw()
{
    try
    {
	getMapById(map->id());
	return true;
    }
    catch (const NotFound&)
    {
	return false;
    }
}

void Database::removeMapsByTerrain(Terrain* terrain)
{
    Maps::iterator i;

    // cascade the delete to events
    for (i = allMaps.begin(); i != allMaps.end(); ++i)
    {
	Map* map = *i;
	if (map->terrainId() == terrain->id())
	    removeEventsByMap(map);
    }

    // remove all matching maps
    i = std::remove_if(allMaps.begin(), allMaps.end(),
		       compare_fun(&Map::terrainId, terrain->id()));
    allMaps.erase(i, allMaps.end());
}

Map* Database::getMapByName(const std::string& name) const throw(NotFound)
{
    Maps::const_iterator i;
    i = std::find_if(allMaps.begin(), allMaps.end(), 
		     compare_fun(&Map::name, name));
    if (i != allMaps.end())
	return *i;
    else
	throw NotFound("Map not found");
}

Map* Database::getMapById(const std::string& id) const throw(NotFound)
{
    Maps::const_iterator i;
    i = std::find_if(allMaps.begin(), allMaps.end(), 
		     compare_fun(&Map::id, id));
    if (i != allMaps.end())
	return *i;
    else
	throw NotFound("Map not found");
}

Map* Database::getMapByTerrainId(const std::string& terrainId) const throw(NotFound)
{
    Maps::const_iterator i;
    i = std::find_if(allMaps.begin(), allMaps.end(), 
		     compare_fun(&Map::terrainId, terrainId));
    if (i != allMaps.end())
	return *i;
    else
	throw NotFound("Map not found");
}

void Database::setSelectedMap(Map* map) throw(NotFound)
{
    Maps::const_iterator i;
    i = std::find(selectedMaps.begin(), selectedMaps.end(), map);
    int index = (i != selectedMaps.end()) ? i - selectedMaps.begin() : -1;
    if (index == -1)
	throw NotFound("Map not found");
    setSelectedMapIndex(index);
}

Map* Database::getSelectedMap() const throw(NotFound)
{
    try 
    {
	return selectedMaps.at(selectedMapIndex);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("No map selected");
    }
}

int Database::getSelectedMapIndex() const throw()
{
    return selectedMapIndex;
}

void Database::setSelectedMapIndex(int index) throw()
{
    selectedMapIndex = index;
}

char** Database::getSelectedMaps() const
{
    return maps;
}

Map* Database::getRandomMap() const throw(NotFound)
{
    Maps::const_iterator iter = selectedMaps.begin();
    if (iter != selectedMaps.end())
	return *iter;
    else
	throw NotFound("Map not found");
}

void Database::freeSelectedMaps()
{
    for (int i = 0; i < selectedMapsSize; i++)
	delete[] maps[i];

    delete[] maps;
    selectedMapsSize = 0;
    maps = 0;
}

/* construct a list of maps whose terrain id is equal to
 * the terrain id of the currently selected terrain */
void Database::updateMapsView()
{
    selectedMaps.clear();
    try
    {
	Terrain* terrain = getSelectedTerrain();
	copy_if(allMaps.begin(), allMaps.end(), std::back_inserter(selectedMaps), 
		compare_fun(&Map::terrainId, terrain->id()));
    }
    catch (const NotFound&)
    {
    }
    
    if (maps != 0)
	freeSelectedMaps();

    maps = new char*[selectedMaps.size()+1];
    selectedMapsSize = 0;
    for (Maps::const_iterator i = selectedMaps.begin(); i != selectedMaps.end(); i++)
    {
	Map* map = *i;
	//std::string name = map->name();
	std::string name = map->toString();
	const char* mapName = name.c_str();
	maps[selectedMapsSize] = new char[strlen(mapName)+1];
	strcpy(maps[selectedMapsSize++], mapName);
    }
    maps[selectedMapsSize] = 0;
}

// return map of the selected event
Map* Database::getEventMap() const throw(NotFound)
{
    try
    {
	Event* event = getSelectedEvent();
	return getMapById(event->mapId());
    }
    catch (const NotFound&)
    {
	return NULL;
    }

}

/**************/


void Database::updateCourseObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::coursesUpdate(), callData));
}

void Database::adjustSelectedCourse()
{
    int numCourses = selectedCoursesSize;
    int selectedIndex = getSelectedCourseIndex();

    if (numCourses == 0)
	selectedIndex = -1;
    else if (numCourses == 1)
	selectedIndex = 0;
    else if (selectedIndex >= numCourses)
	selectedIndex = numCourses - 1;
    setSelectedCourseIndex(selectedIndex);
}

std::string Database::getUniqueCourseName(std::string base)
{
    std::string name;
    int i = 1;
    while (true)
    {
        std::ostringstream str;
        str << base << i;
        name = str.str();
        try 
        {
            getCourseByName(name);
            ++i;
            str.clear();
        }
        catch (const NotFound&)
        {
            break;
        }
    }
    return name;
}

void Database::addCourse(Course* course) throw(Exists)
{
    if (exists(course))
        throw Exists("Course exists in database");

    allCourses.push_back(course);
    needSave = true;
}

void Database::removeCourse(Course* course) throw(NotFound)
{
    if (!exists(course))
	throw NotFound("Course does not exist in database");

    // cascade the delete to events
    removeEventsByCourse(course);
    updateEventsView();
    adjustSelectedEvent();
    updateEventObservers(0);

    // remove the course
    Courses::iterator ci = std::remove(allCourses.begin(), allCourses.end(), course);
    allCourses.erase(ci, allCourses.end());

    needSave = true;
}

bool Database::exists(Course* course) const throw()
{
    try
    {
	getCourseById(course->id());
	return true;
    }
    catch (const NotFound&)
    {
	return false;
    }
}

void Database::removeCoursesByTerrain(Terrain* terrain)
{
    Courses::iterator i;

    // cascade the delete to events
    for (i = allCourses.begin(); i != allCourses.end(); ++i)
    {
	Course* course = *i;
	if (course->terrainId() == terrain->id())
	    removeEventsByCourse(course);
    }

    // remove all matching courses
    i = std::remove_if(allCourses.begin(), allCourses.end(),
		       compare_fun(&Course::terrainId, terrain->id()));
    allCourses.erase(i, allCourses.end());
}

Course* Database::getCourseByName(const std::string& name) const throw(NotFound)
{
    Courses::const_iterator i = std::find_if(allCourses.begin(), allCourses.end(), 
					     compare_fun(&Course::name, name));
    if (i != allCourses.end())
	return *i;
    else
	throw NotFound("Course not found");
}

Course* Database::getCourseById(const std::string& id) const throw(NotFound)
{
    Courses::const_iterator i = std::find_if(allCourses.begin(), allCourses.end(), 
					     compare_fun(&Course::id, id));
    if (i != allCourses.end())
	return *i;
    else
	throw NotFound("Course not found");
}

void Database::setSelectedCourse(Course* course) throw(NotFound)
{
    Courses::const_iterator iter = 
	std::find(selectedCourses.begin(), selectedCourses.end(), course);
    int index = (iter != selectedCourses.end()) ? iter - selectedCourses.begin() : -1;
    if (index == -1)
	throw NotFound("Course not found");
    setSelectedCourseIndex(index);
}

Course* Database::getSelectedCourse() const throw(NotFound)
{
    try 
    {
	return selectedCourses.at(selectedCourseIndex);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("No course selected");
    }
}

int Database::getSelectedCourseIndex() const throw()
{
    return selectedCourseIndex;
}

void Database::setSelectedCourseIndex(int index) throw()
{
    selectedCourseIndex = index;
}

char** Database::getSelectedCourses() const throw()
{
    return courses;
}

Course* Database::getRandomCourse() const throw(NotFound)
{
    Courses::const_iterator iter = selectedCourses.begin();
    if (iter != selectedCourses.end())
	return *iter;
    else
	throw NotFound("Course not found");
}

void Database::freeSelectedCourses()
{
    for (int i = 0; i < selectedCoursesSize; i++)
	delete[] courses[i];

    delete[] courses;
    selectedCoursesSize = 0;
    courses = 0;
}

/* constuct a list of courses whose terrain id is equal to
 * the terrain id of the currently selected terrain
 */
void Database::updateCoursesView()
{
    selectedCourses.clear();
    try
    {
	Terrain* terrain = getSelectedTerrain();
	copy_if(allCourses.begin(), allCourses.end(), std::back_inserter(selectedCourses), 
		compare_fun(&Course::terrainId, terrain->id()));
    }
    catch (const NotFound&)
    {
    }
    
    if (courses != 0)
	freeSelectedCourses();

    courses = new char*[selectedCourses.size()+1];
    selectedCoursesSize = 0;
    for (Courses::const_iterator i = selectedCourses.begin(); 
	 i != selectedCourses.end(); i++)
    {
	Course* course = *i;
	//std::string name = course->name();
	std::string name = course->toString();
	const char* courseName = name.c_str();
	courses[selectedCoursesSize] = new char[strlen(courseName)+1];
	strcpy(courses[selectedCoursesSize++], courseName);
    }
    courses[selectedCoursesSize] = 0;
}

// return course of the selected event
Course* Database::getEventCourse() const throw(NotFound)
{
    try
    {
	Event* event = getSelectedEvent();
	return getCourseById(event->courseId());
    }
    catch (const NotFound&)
    {
	return NULL;
    }
}

/**************/

void Database::updateRunObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::runsUpdate(), callData));
}

void Database::addRun(Run* run) throw(Exists)
{
    if (exists(run))
	throw Exists("Run exists in database");

    allRuns.push_back(run);
    needSave = true;
}

void Database::removeRun(Run* run) throw(NotFound)
{
    if (!exists(run))
	throw NotFound("Run does not exist in database");

    Runs::iterator ri = std::remove(allRuns.begin(), allRuns.end(), run);
    allRuns.erase(ri, allRuns.end());
    
    // remove the route file as well
    std::string filename("routes/");
    filename += run->id() + ".xml";
    remove(filename.c_str());

    needSave = true;
}

bool Database::exists(Run* run) const throw()
{
    try
    {
	getRunById(run->id());
	return true;
    }
    catch (const NotFound&)
    {
	return false;
    }
}

void Database::removeRunsByEvent(Event* event)
{
    Runs::iterator i = std::remove_if(allRuns.begin(), allRuns.end(),
				      compare_fun(&Run::eventId, event->id()));
    allRuns.erase(i, allRuns.end());
    needSave = true;
}

void Database::removeRunsByOrienteer(Orienteer* orienteer)
{
    Runs::iterator i = std::remove_if(allRuns.begin(), allRuns.end(),
				      compare_fun(&Run::orienteerId, orienteer->id()));
    allRuns.erase(i, allRuns.end());
    needSave = true;
}

void Database::adjustSelectedRun()
{
    int numRuns = selectedRunsSize;
    int selectedIndex = getSelectedRunIndex();
    selectedIndex = std::min(std::max(0, selectedIndex), numRuns-1);
    setSelectedRunIndex(selectedIndex);
}

Run* Database::getRunByName(const std::string& name) const throw(NotFound)
{
    Runs::const_iterator i = std::find_if(allRuns.begin(), allRuns.end(), 
					  compare_fun(&Run::name, name));
    if (i != allRuns.end())
	return *i;
    else
	throw NotFound("Run does not exist in database");
}

Run* Database::getRunById(const std::string& id) const throw(NotFound)
{
    Runs::const_iterator i = std::find_if(allRuns.begin(), allRuns.end(), 
					  compare_fun(&Run::id, id));
    if (i != allRuns.end())
	return *i;
    else
	throw NotFound("Run does not exist in database");
}

/* return a run by its position in the selectedRuns list */
Run* Database::getRunByPosition(int position) const throw(NotFound)
{
    try 
    {
	return selectedRuns.at(position);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("Run not found");
    }
}

void Database::setSelectedRun(Run* run) throw(NotFound)
{
    Runs::const_iterator iter = std::find(selectedRuns.begin(), selectedRuns.end(), run);
    int index = (iter != selectedRuns.end()) ? iter - selectedRuns.begin() : -1;
    if (index == -1)
	throw NotFound("Run not found");
    setSelectedRunIndex(index);
}

Run* Database::getSelectedRun() const throw(NotFound)
{
    try 
    {
	return selectedRuns.at(selectedRunIndex);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("No run selected");
    }
}

int Database::getSelectedRunIndex() const throw()
{
    return selectedRunIndex;
}

void Database::setSelectedRunIndex(int index) throw()
{
    selectedRunIndex = index;
}

Database::Runs Database::getRuns() const throw()
{
    return allRuns;
}

Database::Runs  Database::getSelectedRuns2() const throw()
{
    return selectedRuns;
}

char** Database::getSelectedRuns() const throw()
{
    return runs;
}

Run* Database::getRandomRun() const throw(NotFound)
{
    Runs::const_iterator iter = selectedRuns.begin();
    if (iter != selectedRuns.end())
	return *iter;
    else
	throw NotFound("Run not found");
}

void Database::freeSelectedRuns()
{
    for (int i = 0; i < selectedRunsSize; i++)
	delete[] runs[i];

    delete[] runs;
    selectedRunsSize = 0;
    runs = 0;
}

void Database::updateRunsView()
{
    selectedRuns.clear();
    try
    {
	Event* event = getSelectedEvent();

	copy_if(allRuns.begin(), allRuns.end(), std::back_inserter(selectedRuns),
		compare_fun(&Run::eventId, event->id()));
	std::sort(selectedRuns.begin(), selectedRuns.end(), run_lt);
    }
    catch (const NotFound&)
    {
    }

    if (runs != 0)
	freeSelectedRuns();

    runs = new char*[selectedRuns.size()+1];
    selectedRunsSize = 0;
    for (Runs::const_iterator i = selectedRuns.begin(); i != selectedRuns.end(); i++)
    {
	Run* run = *i;
	std::string line = run->toString(selectedRunsSize+1);
	runs[selectedRunsSize] = new char[line.size()+1];
	const char* ln = line.c_str();
	strcpy(runs[selectedRunsSize++], ln);
    }
    runs[selectedRunsSize] = 0;
}

/**************/

void Database::updateOrienteerObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::orienteersUpdate(), callData));
}

void Database::adjustSelectedOrienteer()
{
    int numOrienteers = allOrienteers.size();
    int selectedIndex = getSelectedOrienteerIndex();

    if (numOrienteers == 0)
	selectedIndex = -1;
    else if (numOrienteers == 1)
	selectedIndex = 0;
    else if (selectedIndex >= numOrienteers)
	selectedIndex = numOrienteers - 1;
    setSelectedOrienteerIndex(selectedIndex);
}

void Database::addOrienteer(Orienteer* orienteer) throw(Exists)
{
    if (exists(orienteer))
	throw Exists("Orienteer exists in database");

    allOrienteers.push_back(orienteer);
    needSave = true;
}

void Database::removeOrienteer(Orienteer* orienteer) throw(NotFound)
{
    if (!exists(orienteer))
	throw NotFound("Orienteer does not exist in database");

    removeRunsByOrienteer(orienteer);
    updateRunsView();
    adjustSelectedRun();
    updateRunObservers(0);

    // remove the orienteer
    Orienteers::iterator i =
	std::remove(allOrienteers.begin(), allOrienteers.end(), orienteer);
    allOrienteers.erase(i, allOrienteers.end());

    needSave = true;
}

bool Database::exists(Orienteer* orienteer) const throw()
{
    try
    {
	getOrienteerById(orienteer->id());
	return true;
    }
    catch (const NotFound&)
    {
	return false;
    }
}

Orienteer* Database::getOrienteerByName(const std::string& name) const throw(NotFound)
{
    Orienteers::const_iterator i = 
	std::find_if(allOrienteers.begin(), allOrienteers.end(), 
		     compare_fun(&Orienteer::name, name));
    if (i != allOrienteers.end())
	return *i;
    else
	throw NotFound("Orienteer not found");
}

Orienteer* Database::getOrienteerById(const std::string& id) const throw(NotFound)
{
    Orienteers::const_iterator i = 
	std::find_if(allOrienteers.begin(), allOrienteers.end(), 
		     compare_fun(&Orienteer::id, id));
    if (i != allOrienteers.end())
	return *i;
    else
	throw NotFound("Orienteer not found");
}

void Database::setSelectedOrienteer(Orienteer* orienteer) throw(NotFound)
{
    Orienteers::const_iterator i = 
	std::find(allOrienteers.begin(), allOrienteers.end(), orienteer);
    int index = (i != allOrienteers.end()) ? i - allOrienteers.begin() : -1;
    if (index == -1)
	throw NotFound("Orienteer not found");
    setSelectedOrienteerIndex(index);
}

int Database::getSelectedOrienteerIndex() const throw()
{
    return selectedOrienteerIndex;
}

void Database::setSelectedOrienteerIndex(int index) throw()
{
    selectedOrienteerIndex = index;
}

Orienteer* Database::getSelectedOrienteer() const throw(NotFound)
{
    try 
    {
	return allOrienteers.at(selectedOrienteerIndex);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("No orienteer selected");
    }
}

char** Database::getSelectedOrienteers() const throw()
{
    return orienteers;
}

Orienteer* Database::getRandomOrienteer() const throw(NotFound)
{
    Orienteers::const_iterator iter = allOrienteers.begin();
    if (iter != allOrienteers.end())
	return *iter;
    else
	throw NotFound("Orienteer not found");
}

void Database::freeSelectedOrienteers()
{
    for (int i = 0; i < selectedOrienteersSize; i++)
	delete[] orienteers[i];

    delete[] orienteers;
    selectedOrienteersSize = 0;
    orienteers = 0;
}

/* constuct a list of courses whose terrain id is equal to
 * the terrain id of the currently selected terrain
 */
void Database::updateOrienteersView()
{
    if (orienteers != 0)
	freeSelectedOrienteers();

    orienteers = new char*[allOrienteers.size() + 1];
    selectedOrienteersSize = 0;
    for (Orienteers::const_iterator i = allOrienteers.begin(); 
	 i != allOrienteers.end(); i++)
    {
	Orienteer* orienteer = *i;
	std::string name = orienteer->name();
	const char* orienteerName = name.c_str();
	orienteers[selectedOrienteersSize] = new char[strlen(orienteerName)+1];
	strcpy(orienteers[selectedOrienteersSize++], orienteerName);
    }
    orienteers[selectedOrienteersSize] = 0;
}

/**************/

void Database::updateEventObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::eventsUpdate(), callData));
}

void Database::adjustSelectedEvent()
{
    int numEvents = allEvents.size();
    int selectedIndex = getSelectedEventIndex();

    if (numEvents == 0)
	selectedIndex = -1;
    else if (numEvents == 1)
	selectedIndex = 0;
    else if (selectedIndex >= numEvents)
	selectedIndex = numEvents - 1;
    setSelectedEventIndex(selectedIndex);
}

std::string Database::getUniqueEventName(std::string base)
{
    std::string name;
    int i = 1;
    while (true)
    {
        std::ostringstream str;
        str << base << i;
        name = str.str();
        try 
        {
            getEventByName(name);
            ++i;
            str.clear();
        }
        catch (const NotFound&)
        {
            break;
        }
    }
    return name;
}

void Database::addEvent(Event* event) throw(Exists)
{
    if (exists(event))
	throw Exists("Event is already in database");

    allEvents.push_back(event);
    needSave = true;
}

bool Database::exists(Event* event) const throw()
{
    try
    {
	getEventById(event->id());
	return true;
    }
    catch (const NotFound&)
    {
	return false;
    }
}

Event* Database::getRandomEvent() const  throw(NotFound)
{
    Events::const_iterator iter = allEvents.begin();
    if (iter != allEvents.end()) 
	return *iter;
    else
	throw NotFound("Can not find event");
}

void Database::removeEvent(Event* event) throw(NotFound)
{
    if (!exists(event))
	throw NotFound("Can not find event");

    // cascade the delete to runs
    removeRunsByEvent(event);
    updateRunsView();
    adjustSelectedRun();
    updateRunObservers(0);

    // remove the event
    Events::iterator ei = std::remove(allEvents.begin(), allEvents.end(), event);
    allEvents.erase(ei, allEvents.end());

    needSave = true;
}

void Database::removeEventsByMap(Map* map)
{
    Events::iterator i;

    // cascade the delete to runs
    for (i = allEvents.begin(); i != allEvents.end(); ++i)
    {
	Event* event = *i;
	if (event->mapId() == map->id())
	{
	    removeRunsByEvent(event);
	}
    }
    updateRunsView();
    adjustSelectedRun();
    updateRunObservers(0);

    // delete the event
    i = std::remove_if(allEvents.begin(), allEvents.end(),
		       compare_fun(&Event::mapId, map->id()));
    allEvents.erase(i, allEvents.end());
}

void Database::removeEventsByCourse(Course* course)
{
    Events::iterator i;

    // cascade the delete to runs
    for (i = allEvents.begin(); i != allEvents.end(); ++i)
    {
	Event* event = *i;
	if (event->courseId() == course->id())
	{
	    removeRunsByEvent(event);
	}
    }
    updateRunsView();
    adjustSelectedRun();
    updateRunObservers(0);

    // delete the event
    i = std::remove_if(allEvents.begin(), allEvents.end(),
		       compare_fun(&Event::courseId, course->id()));
    allEvents.erase(i, allEvents.end());
}

Event* Database::getEventByName(const std::string& name) const throw(NotFound)
{
    Events::const_iterator i;
    i= std::find_if(allEvents.begin(), allEvents.end(), 
		    compare_fun(&Event::name, name));
    if (i != allEvents.end())
	return *i;
    else
	throw NotFound("Can not find event");
}

Event* Database::getEventById(const std::string& id) const throw(NotFound)
{
    Events::const_iterator i;
    i = std::find_if(allEvents.begin(), allEvents.end(), 
		     compare_fun(&Event::id, id));
    if (i != allEvents.end())
	return *i;
    else
	throw NotFound("Can not find event");
}

Event* Database::getEventByCourseId(const std::string& courseId) const throw(NotFound)
{
    Events::const_iterator i;
    i = std::find_if(allEvents.begin(), allEvents.end(), 
		     compare_fun(&Event::courseId, courseId));
    if (i != allEvents.end())
	return *i;
    else
	throw NotFound("Can not find event");
}

Event* Database::getEventByMapId(const std::string& mapId) const throw(NotFound)
{
    Events::const_iterator i;
    i = std::find_if(allEvents.begin(), allEvents.end(), 
		     compare_fun(&Event::mapId, mapId));
    if (i != allEvents.end())
	return *i;
    else
	throw NotFound("Can not find event");
}

void Database::setSelectedEvent(Event* event) throw(NotFound)
{
    Events::const_iterator i = std::find(allEvents.begin(), allEvents.end(), event);
    int index = (i != allEvents.end()) ? i - allEvents.begin() : -1;
    if (index == -1)
	throw NotFound("Can not find event");
    setSelectedEventIndex(index);
}

int Database::getSelectedEventIndex() const throw()
{
    return selectedEventIndex;
}

void Database::setSelectedEventIndex(int index) throw(NotFound)
{
    selectedEventIndex = index;
    updateRunsView();
    adjustSelectedRun();
    updateRunObservers(0);
}

Event* Database::getSelectedEvent() const throw(NotFound)
{
    try 
    {
	return allEvents.at(selectedEventIndex);
    }
    catch (std::out_of_range&)
    {
	throw NotFound("No event selected");
    }
}

char** Database::getSelectedEvents() const throw()
{
    return events;
}

void Database::freeSelectedEvents()
{
    for (int i = 0; i < selectedEventsSize; i++)
	delete[] events[i];

    delete[] events;
    selectedEventsSize = 0;
    events = 0;
}

// the pui puListBox widget requires the shown items as C char arrays
void Database::updateEventsView()
{
    if (events != 0)
	freeSelectedEvents();

    events = new char*[allEvents.size() + 1];
    selectedEventsSize = 0;
    for (Events::const_iterator i = allEvents.begin(); i != allEvents.end(); i++)
    {
	Event* event = *i;
	std::string name = event->name();
	const char* eventName = name.c_str();
	events[selectedEventsSize] = new char[strlen(eventName)+1];
	strcpy(events[selectedEventsSize++], eventName);
    }
    events[selectedEventsSize] = 0;
}

// event we have received over the network
void Database::addMPEvent(Terrain* terrain, Map* map, Course* course, Event* event)
{
    try
    {
	Terrain* t = getTerrainById(terrain->id());
        setSelectedTerrain(t);
    }
    catch (const NotFound&)
    {
	addTerrain(terrain);
        //setSelectedTerrain(terrain);
    }

    try
    {
	Map* m = getMapById(map->id());
	setSelectedMap(m);
    }
    catch (const NotFound&)
    {
	addMap(map);
	//setSelectedMap(map);
    }

    try
    {
	Course* c = getCourseById(course->id());
	setSelectedCourse(c);
    }
    catch (const NotFound&)
    {
	addCourse(course);
	//setSelectedCourse(course);
    }

    try
    {
	Event* e = getEventById(event->id());
        setSelectedEvent(e);
    }
    catch (const NotFound&)
    {
	addEvent(event);
        setSelectedEvent(event);
    }
}

/**************/

void Database::updateServerObservers(void* callData) const
{
    std::for_each(observers.begin(), observers.end(), std::bind2nd(DatabaseObserver::serversUpdate(), callData));
}

void Database::addServer(const std::string& hostname)
{
    ServerInfo* si = geServerByHostname(hostname);
    time_t now = time(NULL);

    if (si != 0)
	si->lastUsed(now);
    else
	addServer(new ServerInfo(hostname, now));
    std::sort(mruServers.begin(), mruServers.end());
    updateServersView();
    updateServerObservers(0);
}

ServerInfo* Database::geServerByHostname(const std::string& hostname) const
{
    Servers::const_iterator i =
	std::find_if(mruServers.begin(), mruServers.end(),
		     compare_fun(&ServerInfo::hostname, hostname));
    return (i != mruServers.end()) ? *i : 0;
}

void Database::addServer(ServerInfo* serverInfo)
{
    mruServers.push_back(serverInfo);
    //updateServersView();
    //updateServerObservers(0);
}

char** Database::getServers() const
{
    return servers;
}

void Database::freeServers()
{
    for (int i = 0; i < serversSize; i++)
	delete[] servers[i];

    delete[] servers;
    serversSize = 0;
    servers = 0;
}

// the pui puListBox widget requires the shown items as C char arrays
void Database::updateServersView()
{
    if (servers != 0)
	freeServers();

    servers = new char*[mruServers.size() + 1];
    serversSize = 0;
    for (Servers::const_iterator i = mruServers.begin(); i != mruServers.end(); i++)
    {
	ServerInfo* serverInfo = *i;
	std::string name = serverInfo->hostname();
	const char* hostName = name.c_str();
	servers[serversSize] = new char[strlen(hostName)+1];
	strcpy(servers[serversSize++], hostName);
    }
    servers[serversSize] = 0;
}

#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <istream>
#include <fstream>
#include <stdexcept>
#include <ctime>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "database.h"
#include "run.h"
#include "util.h"
#include "mesh.h"
#include "xmlParser.h"

using batb::XmlParser;

#define DISTANCE_COEFF 3


std::ostream& SplitTime::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<splitTime>\n";
    ostr << i << "  <time>" << time << "</time>\n";
    ostr << i << "  <courseLength>" << courseLength << "</courseLength>\n";
    ostr << i << "  <routeLength>" << routeLength << "</routeLength>\n";
    ostr << i << "</splitTime>\n";

    return ostr;
}

bool operator<(const Run& run1, const Run& run2)
{
    Run::RunStatus s1 = run1.getStatus();
    Run::RunStatus s2 = run2.getStatus();
    fptype t1 = run1.getTotalTime(); // time in seconds
    fptype t2 = run2.getTotalTime();

    // add penalties
    if (s1 == Run::DQF)
    {
	t1 += 200000;
    }
    else if (s1 == Run::DNF)
    {
	t1 += 100000;
    }

    if (s2 == Run::DQF)
    {
	t2 += 200000;
    }
    else if (s2 == Run::DNF)
    {
	t2 += 100000;
    }

    return t1 < t2;
}

bool run_lt(const Run* run1, const Run* run2)
{
    return *run1 < *run2;
}

fptype Run::getTotalTime() const
{
    return _totalTime;
}

Run::Run(std::string id, std::string name, std::string eventId, std::string orienteerId,
    time_t startTime, time_t stopTime, RunStatus status)
    : _name(name), _eventId(eventId), _orienteerId(orienteerId), _id(id),
      _startTime(startTime), _stopTime(stopTime), _status(status)
{
    _totalTime = difftime(stopTime, startTime);
}

Run::Run(std::string name, std::string eventId, std::string orienteerId)
    : _name(name), _eventId(eventId), _orienteerId(orienteerId), _id(genId())
{
}

Run::~Run()
{
    std::for_each(route.begin(), route.end(), del_fun<Point>());
    std::for_each(splitTimes.begin(), splitTimes.end(), del_fun<SplitTime>());
}

std::string Run::statusAsString() const
{
    switch (_status)
    {
    case Run::OK: return "OK";
    case Run::DNF: return "DNF";
    case Run::DQF: return "DQF";
    }
    return "OK";
}

std::string Run::toString(int position) const
{
    Database& db = Database::instance();
    Orienteer* orienteer = db.getOrienteerById(orienteerId());
    std::string name = orienteer->name();
    
    if (_status == Run::OK)
    {
	fptype totalTime = getTotalTime();
	int seconds = int(totalTime)%60;
	int minutes = int(totalTime)/60;
	std::ostringstream ostr;
	ostr << position << " " << name << " " << minutes << ":" << seconds;
	return ostr.str();
    }
    else
    {
	std::string status = statusAsString();
	std::string line = status + " " + name;
	return line;
    }
}

int Run::numTimes()
{
    return splitTimes.size();
}

time_t Run::stopTime()
{
    return _stopTime;
}

time_t Run::startTime()
{
    return _startTime;
}

void Run::recordSplitTime(SplitTime* st)
{
    splitTimes.push_back(st);
}

std::string Run::genId()
{
    std::ostringstream ostr;

    ostr << _orienteerId
	 << _eventId
	 << _startTime
	 << _stopTime;

    return std::string(md5sum(ostr.str().c_str()));
}

Run::RunStatus Run::getStatus() const
{
    return _status;
}

void Run::setStatus(RunStatus s)
{
    _status = s;
}

void Run::setTimes(time_t startTime, time_t stopTime)
{
    _startTime = startTime;
    _stopTime = stopTime;
}

void Run::start()
{
    _totalDirect = 0;
    _startTime = time(NULL);
    recordSplitTime(new SplitTime(0,0,0));
}

void Run::punch(Control* prevControl, Control* thisControl, fptype length)
{
    fptype splitDirect = thisControl->position.distance(prevControl->position);
    _totalDirect += splitDirect;
    time_t now = time(NULL);
    _totalTime = difftime(now, _startTime);
    SplitTime* splitTime = new SplitTime(_totalTime, _totalDirect, trackLength());
    recordSplitTime(splitTime);
}

void Run::finish()
{
    _stopTime = time(NULL);
    _id = genId();
}

void Run::recordPosition(const Point* const point)
{
    route.push_back(new Point(*point));
}

fptype Run::trackLength()
{
    fptype result = 0.0;

    if (route.size() > 1)
    {
	for (std::vector<Point*>::const_iterator i = route.begin();
	     i != route.end(); ++i)
	{
	    Point* thisPoint = *i;
	    std::vector<Point*>::const_iterator j = i+1;
	    if (j != route.end())
	    {
		Point* nextPoint = *j;
		result += thisPoint->distance(*nextPoint);
	    }
	}
    }

    return result;
}

void Run::draw(bool highlight)
{
    if (route.empty())
    {
	const XmlParser& xmlParser = XmlParser::instance();
	xmlParser.readRoute(this);
    }

    if (route.size() < 2)
	return;

    glPushMatrix();
    if (highlight)
	glColor3f(0.0, 1.0, 0.0);
    else
	glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_LINE_STRIP);
    std::for_each(route.begin(), route.end(), std::mem_fun(&Point::_glVertex2f));
    glEnd();

    glPopMatrix();
}

bool Run::fileExists(std::string filename) const
{
    std::ifstream istr(filename.c_str());
    if (istr)
    {
	istr.close();
	return true;
    }
    else
    {
	return false;
    }
}

void Run::writeRoute() const 
{
    std::string filename("routes/");
    filename += _id + ".xml";
    if (!fileExists(filename))
    {
	std::ofstream ostr(filename.c_str(), std::ios::out | std::ios::binary);
	writeRoute(ostr, 0);
	ostr.close();
    }
}

void Run::writeRoute(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');
    ostr << i << "<route>\n";
    std::for_each(route.begin(), route.end(), printXML<Point*>(ostr, indent+4));
    ostr << i << "</route>\n";
}

std::ostream& Run::toXML(std::ostream& ostr, int indent) const 
{
    std::string i(indent, ' ');

    ostr << i << "<run>\n";

    ostr << i << "  <id>" << _id << "</id>\n";
    ostr << i << "  <name>" << _name << "</name>\n";
    ostr << i << "  <eventId>" << _eventId << "</eventId>\n";
    ostr << i << "  <orienteerId>"  << _orienteerId << "</orienteerId>\n";
    ostr << i << "  <startTime>" << _startTime << "</startTime>\n";
    ostr << i << "  <stopTime>" << _stopTime << "</stopTime>\n";
    ostr << i << "  <status>" << _status << "</status>\n";

    writeRoute();

    ostr << i << "  <times>\n";
    std::for_each(splitTimes.begin(), splitTimes.end(), printXML<SplitTime*>(ostr, indent+4));
    ostr << i << "  </times>\n";

    ostr << i << "</run>\n";

    return ostr;
}

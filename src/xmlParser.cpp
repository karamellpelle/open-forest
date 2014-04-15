#include <string>
#include <iostream>
#include <ostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <map>

#include "run.h"
#include "orienteer.h"
#include "terrain.h"
#include "map.h"
#include "course.h"
#include "control.h"
#include "event.h"
#include "message.h"
#include "database.h"
#include "xmlParser.h"

using batb::TokenStream;
using batb::Token;
using batb::XmlParser;
using batb::TokenType;


const char* tokenTypeToString(batb::TokenType type)
{
    switch (type) 
    {
    case batb::TT_xmlBeginTag: return "xmlBeginTag"; break;
    case batb::TT_xmlEndTag: return "xmlEndTag"; break;
    case batb::TT_integer: return "integer"; break;
    case batb::TT_real: return "real"; break;
    case batb::TT_boolean: return "boolean"; break;
    case batb::TT_string: return "string"; break;
    case batb::TT_eof: return "eof"; break;
    case batb::TT_error: return "ERROR"; break;
    default: return "ERROR"; break;
    }
}

std::ostream& operator<<(std::ostream& ostr, batb::TokenType& tokenType)
{
    switch (tokenType) 
    {
    case batb::TT_xmlBeginTag: ostr << "xmlBeginTag"; break;
    case batb::TT_xmlEndTag: ostr << "xmlEndTag"; break;
    case batb::TT_integer: ostr << "integer"; break;
    case batb::TT_real: ostr << "real"; break;
    case batb::TT_boolean: ostr << "boolean"; break;
    case batb::TT_string: ostr << "string"; break;
    case batb::TT_eof: ostr << "eof"; break;
    case batb::TT_error: ostr << "ERROR"; break;
    default: ostr << "ERROR"; break;
    }
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, batb::Token& token)
{
    ostr << token.type << " " << token.value;
    return ostr;
}

TokenStream::TokenStream(std::streambuf* buffer)
    : std::istream(buffer), lineNumber(1), column(0)
{
}

int TokenStream::get()
{
    int c = std::istream::get();
    currentLine += c;
    column++;
    if (c == '\n')
    {
	currentLine.clear();
	lineNumber++;
	column = 0;
    }
    return c;
}

void TokenStream::error(std::string msg)
{
    std::cout << lineNumber << ": " << msg << std::endl;
    std::cout << currentLine << std::endl;
    for (int i = 0; i < column; i++)
	std::cout << ' ';
    std::cout << "^" << std::endl;
}

TokenStream& TokenStream::operator>>(Token& token)
{
    TokenType type;
    std::string value;
    int c;

    while (isspace(c = peek())) 
	c = get();

    c = peek();
    if (c == '<') // xml tag
    {
	c = get(); // throw away <
	c = peek();
	if (c == '/') 
	{
	    type = TT_xmlEndTag;
	    c = get(); // throw away /
	}
	else
	{
	    type = TT_xmlBeginTag;
	}
	while (isalnum(c = peek())) 
	    value += get();

	if (c == '>')
	    c = get(); // throw away >
	else
	    error("xml tag not terminated properly");
    } 
    else if (isdigit(c) || (c == '-')) 
    {
	int numdots = 0;

	type = TT_integer;
	while (isalnum(c = peek()) || (c == '.') || (c == '-')) 
	{
	    if (c == '.')
		numdots++;
	    if ((c == '.') && (type == TT_integer))
		type = TT_real;
            if (isalpha(c))
	    {
		// it may be a string even when it starts with a digit
		// md5 sums are like this
                type = TT_string; 
	    }
	    value += get();
	}
	if (numdots > 1)
	{
	    // ip addresses is like this: it has digits 
	    // and multiple dots, we want it as a string
	    type = TT_string; 
	}
    } 
    else if (c == EOF) 
    {
	type = TT_eof;
    }
    else if (isalpha(c)) 
    {
	type = TT_string;

	while (isalnum(c = peek()) || (c == '.')) 
	{
	    value += get();
	}

	if (value == "true" || value == "false")
	{
	    type = TT_boolean;
	}
    } 
    else 
    {
	type = TT_error;
    }
    token.type = type;
    token.value = value;

    return *this;
}

bool XmlParser::parse(TokenStream& input, TokenType type, const char* xmlTag, Token& token) const
{
    Token tok(TT_error, "");
    std::string errmsg;

    input >> tok;
    if (tok.match(TT_xmlBeginTag, xmlTag)) 
    {
	input >> token;
	if (token.match(type)) 
	{
	    input >> tok;
	    if (tok.match(TT_xmlEndTag, xmlTag)) 
		return true;
	    else
	    {
		errmsg += "No ";
		errmsg += xmlTag;
		errmsg += " end tag found";
	    }
	} 
	else
	{
	    errmsg += "No ";
	    errmsg += tokenTypeToString(type);
	    errmsg += " found";
	}
    } 
    else
    {
	errmsg += "No ";
	errmsg += xmlTag;
	errmsg += " begin tag found";
    }
    input.error(errmsg);
    return false;
}

bool XmlParser::parse(TokenStream& input, const char* xmlTag, std::string& s) const
{
    Token token(TT_error, "");

    if (parse(input, TT_string, xmlTag, token))
    {
	s = token.value;
	return true;
    }
    return false;
}

bool XmlParser::parse(TokenStream& input, const char* xmlTag, fptype& f) const
{
    Token token(TT_error, "");

    if (parse(input, TT_real, xmlTag, token))
    {
	f = atof(token.value.c_str());
	return true;
    }
    return false;
}

bool XmlParser::parse(TokenStream& input, const char* xmlTag, int& i) const
{
    Token token(TT_error, "");

    if (parse(input, TT_integer, xmlTag, token))
    {
	i = atoi(token.value.c_str());
	return true;
    }
    return false;
}

#ifndef FREEBSD
bool XmlParser::parse(TokenStream& input, const char* xmlTag, time_t& t) const
{
    Token token(TT_error, "");

    if (parse(input, TT_integer, xmlTag, token))
    {
	t = atoi(token.value.c_str());
	return true;
    }
    return false;
}
#endif

bool XmlParser::parse(TokenStream& input, const char* xmlTag, bool& b) const
{
    Token token(TT_error, "");

    if (parse(input, TT_boolean, xmlTag, token))
    {
	b = (token.value == "true") ? true : false;
	return true;
    }
    return false;
}

bool XmlParser::parse(TokenStream& input, const char* xmlTag, Run::RunStatus& r) const
{
    Token token(TT_error, "");

    if (parse(input, TT_integer, xmlTag, token))
    {
	r = Run::RunStatus(atoi(token.value.c_str()));
	return true;
    }
    return false;
}

bool XmlParser::parse(TokenStream& input, Terrain*& terrain) const
{
    Token token(TT_error, "");

    std::string id;
    std::string name;
    int width, height, altitudeVariation, seed;
    int nonFractalIterations, recursionLevels;
    fptype fractalDimension;

    if (!parse(input, "id", id))
	return false;
    if (!parse(input, "name", name))
	return false;
    if (!parse(input, "width", width))
	return false;
    if (!parse(input, "height", height))
	return false;
    if (!parse(input, "altitudeVariation", altitudeVariation))
	return false;
    if (!parse(input, "seed", seed))
	return false;
    if (!parse(input, "nonFractalIterations", nonFractalIterations))
	return false;
    if (!parse(input, "recursionLevels", recursionLevels))
	return false;
    if (!parse(input, "fractalDimension", fractalDimension))
	return false;

    input >> token;
    if (token.match(TT_xmlEndTag, "terrain"))
    {
	terrain = new Terrain(name, id, width, height, seed,
			      altitudeVariation, nonFractalIterations,
			      recursionLevels, float(fractalDimension));
    } 
    else 
    {
	std::string errmsg = "No terrain end tag found";
	input.error(errmsg);
    }

    return true;
}

bool XmlParser::parse(TokenStream& input, Map*& map) const
{
    Token token(TT_error, "");
    std::string id;
    std::string name;
    std::string terrainId;
    int contourInterval;
    
    if (!parse(input, "id", id))
	return false;
    if (!parse(input, "name", name))
	return false;
    if (!parse(input, "terrainId", terrainId))
	return false;
    if (!parse(input, "contourInterval", contourInterval))
	return false;

    input >> token;
    if (token.match(TT_xmlEndTag, "map"))
    {
	map = new Map(id, name, terrainId, contourInterval);
    } 
    else 
    {
	std::string errmsg = "No map end tag found";
	input.error(errmsg);
    }
    return true;
}

bool XmlParser::parse(TokenStream& input, Point*& point) const
{
    Token token(TT_error, "");
    fptype x, y, z;

    if (!parse(input, "x", x))
	return false;
    if (!parse(input, "y", y))
	return false;
    if (!parse(input, "z", z))
	return false;
    
    input >> token;
    if (token.match(TT_xmlEndTag, "point"))
    {
	point = new Point(x, y, z);
    } 
    else 
    {
	std::string errmsg = "No point end tag found";
	input.error(errmsg);
    }
    return true;
}

bool XmlParser::parse(TokenStream& input, SplitTime*& st) const
{
    Token token(TT_error, "");
    fptype time;
    fptype courseLength;
    fptype routeLength;

    if (!parse(input, "time", time))
	return false;
    if (!parse(input, "courseLength", courseLength))
	return false;
    if (!parse(input, "routeLength", routeLength))
	return false;
    
    input >> token;
    if (token.match(TT_xmlEndTag, "splitTime"))
    {
	st = new SplitTime(time, courseLength, routeLength);
    } 
    else 
    {
	std::string errmsg = "No splitTime end tag found";
	input.error(errmsg);
    }
    return true;
}

bool XmlParser::parse(TokenStream& input, Course*& course) const
{
    Token token(TT_error, "");
    std::string id;
    std::string name;
    std::string terrainId;
#if 0
    int numControls;
    int length;
#endif

    if (!parse(input, "id", id))
	return false;
    if (!parse(input, "name", name))
	return false;
    if (!parse(input, "terrainId", terrainId))
	return false;
#if 0
    if (!parse(input, "numControls", numControls))
	return false;
    if (!parse(input, "length", length))
	return false;
#endif

    course = new Course(id, name, terrainId);

    input >> token;
    if (token.match(TT_xmlBeginTag, "controls"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "point"); 
	     input >> token)
	{
	    Point* point = 0;
	    parse(input, point);
	    if (point != 0)
	    {
		Control* control = new Control(*point);
		course->addControl(control);
		delete point;
	    }
	}
    }
    
    input >> token;
    if (!token.match(TT_xmlEndTag, "course"))
    {
	std::string errmsg = "No course end tag found";
	input.error(errmsg);
    }
    return true;
}

bool XmlParser::parseRoute(TokenStream& input, Run*& run) const
{
    Token token(TT_error, "");
    input >> token;
    if (token.match(TT_xmlBeginTag, "route"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "point"); 
	     input >> token)
	{
	    Point* point;
	    parse(input, point);
	    if (point != 0)
	    {
		run->recordPosition(point);
	    }
	}
    }
    return true;
}

void XmlParser::readRoute(Run* run) const
{
    std::string filename("routes/");
    filename += run->id() + ".xml";
    std::filebuf buffer;
    buffer.open(filename.c_str(), std::ios::in);
    TokenStream input(&buffer);
    Token token(TT_error, "");
    parseRoute(input, run);
    buffer.close();
}

bool XmlParser::parse(TokenStream& input, Run*& run) const
{
    Token token(TT_error, "");
    std::string id;
    std::string name;
    std::string eventId;
    std::string orienteerId;
    time_t startTime;
    time_t stopTime;
    Run::RunStatus status;

    if (!parse(input, "id", id))
	return false;
    if (!parse(input, "name", name))
	return false;
    if (!parse(input, "eventId", eventId))
	return false;
    if (!parse(input, "orienteerId", orienteerId))
	return false;
    if (!parse(input, "startTime", startTime))
	return false;
    if (!parse(input, "stopTime", stopTime))
	return false;
    if (!parse(input, "status", status))
	return false;

    run = new Run(id, name, eventId, orienteerId, startTime, stopTime, status);

    parseRoute(input, run);

    input >> token;
    if (token.match(TT_xmlBeginTag, "times"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "splitTime"); 
	     input >> token)
	{
	    SplitTime* splitTime;
	    parse(input, splitTime);
	    if (splitTime != 0)
	    {
		run->recordSplitTime(splitTime);
	    }
	}
    }
    
    input >> token;
    if (!token.match(TT_xmlEndTag, "run"))
    {
	std::string errmsg = "No run end tag found";
	input.error(errmsg);
    }
    return true;
}

bool XmlParser::parse(TokenStream& input, Orienteer*& orienteer) const
{
    Token token(TT_error, "");
    std::string id;
    std::string name;

    if (!parse(input, "id", id))
	return false;
    if (!parse(input, "name", name))
	return false;

    input >> token;
    if (token.match(TT_xmlEndTag, "orienteer")) 
    {
	orienteer = new Orienteer(name, id);
    }
    else 
    {
	std::string errmsg = "No orienteer end tag found";
	input.error(errmsg);
    }

    return true;
}

bool XmlParser::parse(TokenStream& input, Event*& event) const
{
    Token token(TT_error, "");
    std::string id;
    std::string name;
    std::string mapId;
    std::string courseId;
    bool night;
    bool fog;

    if (!parse(input, "id", id))
	return false;
    if (!parse(input, "name", name))
	return false;
    if (!parse(input, "mapId", mapId))
	return false;
    if (!parse(input, "courseId", courseId))
	return false;
    if (!parse(input, "night", night))
	return false;
    if (!parse(input, "fog", fog))
	return false;

    input >> token;
    if (token.match(TT_xmlEndTag, "event"))
    {
	event = new Event(id, name, mapId, courseId, night, fog);
    } 
    else 
    {
	std::string errmsg = "No event end tag found";
	input.error(errmsg);
    }
    return true;
}

bool XmlParser::parse(TokenStream& input, ServerInfo*& serverInfo) const
{
    Token token(TT_error, "");

    std::string hostname;
    time_t lastUsed;

    if (!parse(input, "hostname", hostname))
	return false;
    if (!parse(input, "lastUsed", lastUsed))
	return false;

    input >> token;
    if (token.match(TT_xmlEndTag, "server"))
    {
	serverInfo = new ServerInfo(hostname, lastUsed);
    } 
    else 
    {
	std::string errmsg = "No server end tag found";
	input.error(errmsg);
    }
    return true;
}

void XmlParser::parseTerrains(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "terrains"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "terrain"); 
	     input >> token)
	{
	    Terrain* terrain;
	    if (parse(input, terrain))
	    {
		try
		{
		    db.addTerrain(terrain);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	db.updateTerrainsView();
	try
	{
	    Terrain* terrain = db.getRandomTerrain();
	    db.setSelectedTerrain(terrain);
	    db.updateTerrainObservers(0);
	}
	catch (const NotFound&)
	{
	}
    }
}

void XmlParser::parseMaps(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "maps"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "map"); 
	     input >> token)
	{
	    Map* map;
	    if (parse(input, map))
	    {
		try
		{
		    db.addMap(map);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	db.updateMapsView();
	try
	{
	    Map* map = db.getRandomMap();
	    db.setSelectedMap(map);
	    db.updateMapObservers(0);
	}
	catch (const NotFound&)
	{
	}
    }
}

void XmlParser::parseCourses(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "courses"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "course"); 
	     input >> token)
	{
	    Course* course;
	    if (parse(input, course))
	    {
		try
		{
		    db.addCourse(course);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	db.updateCoursesView();
	try
	{
	    Course* course = db.getRandomCourse();
	    db.setSelectedCourse(course);
	    db.updateCourseObservers(0);
	}
	catch (const NotFound&)
	{
	}
    }
}

void XmlParser::parseEvents(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "events"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "event"); 
	     input >> token)
	{
	    Event* event;
	    if (parse(input, event))
	    {
		try
		{
		    db.addEvent(event);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	db.updateEventsView();
	try
	{
	    Event* event = db.getRandomEvent();
	    db.setSelectedEvent(event);
	    db.updateEventObservers(0);
	}
	catch (const NotFound&)
	{
	}
    }
}

void XmlParser::parseOrienteers(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "orienteers"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "orienteer"); 
	     input >> token)
	{
	    Orienteer* orienteer;
	    if (parse(input, orienteer))
	    {
		try
		{
		    db.addOrienteer(orienteer);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	
	//input >> token; 
	if (token.match(TT_xmlBeginTag, "selectedOrienteer"))
	{
	    input >> token; 
	    if (token.match(TT_xmlBeginTag, "orienteer"))
	    {
		Orienteer* orienteer;
		if (parse(input, orienteer))
		{
		    Orienteer* ori;
		    try
		    {
			ori = db.getOrienteerById(orienteer->id());
		    }
		    catch (const NotFound&)
		    {
			ori = db.getRandomOrienteer();
		    }
		    db.setSelectedOrienteer(ori);
		    delete orienteer;
		}

		input >> token;
		if (!token.match(TT_xmlEndTag, "selectedOrienteer")) 
		{
		    std::string errmsg = "No selectedOrienteer end tag found";
		    input.error(errmsg);
		}

	    }
	}

	db.updateOrienteersView();

	input >> token;
	if (!token.match(TT_xmlEndTag, "orienteers")) 
	{
	    std::string errmsg = "No orienteers end tag found";
	    input.error(errmsg);
	}
    }
}

void XmlParser::parseRuns(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "runs"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "run"); 
	     input >> token)
	{
	    Run* run;
	    if (parse(input, run))
	    {
		try
		{
		    db.addRun(run);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	db.updateRunsView();
	try
	{
	    Run* run = db.getRandomRun();
	    db.setSelectedRun(run);
	    db.updateRunObservers(0);
	}
	catch (const NotFound&)
	{
	}
    }
}

void XmlParser::parseServers(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");

    input >> token;
    if (token.match(TT_xmlBeginTag, "servers"))
    {
	for (input >> token; 
	     token.match(TT_xmlBeginTag, "server"); 
	     input >> token)
	{
	    ServerInfo* serverInfo;
	    if (parse(input, serverInfo))
	    {
		try
		{
		    db.addServer(serverInfo);
		}
		catch (const Exists&)
		{
		}
	    }
	}
	try
	{
	    db.updateServersView();
	    db.updateServerObservers(0);
	}
	catch (const NotFound&)
	{
	}
    }
}

void XmlParser::parse(TokenStream& input, Database& db) const
{
    Token token(TT_error, "");
    
    input >> token;
    if (token.match(TT_xmlBeginTag, "database"))
    {
	parseTerrains(input, db);
	parseMaps(input, db);
	parseCourses(input, db);
	parseEvents(input, db);
	parseOrienteers(input, db);
	parseRuns(input, db);
	parseServers(input, db);
    }
}

void XmlParser::readFile(const std::string& filename, Database& db) const
{
    std::filebuf buffer;
    buffer.open(filename.c_str(), std::ios::in);
    TokenStream input(&buffer);
    Token token(TT_error, "");
    parse(input, db);

    buffer.close();
}

#if 0
bool XmlParser::msgRunParse(std::istream& istr, MsgRun*& msgRun) const
{
    std::streambuf* buffer = istr.rdbuf();
    TokenStream input(buffer);
    Token token(TT_error, "");

    input >> token;
    Run* run;
    if (!parse(input, run))
	return false;
    
    msgRun = new MsgRun(run);
    return true;
}
#endif

bool XmlParser::msgEventInfoXMLparse(std::istream& istr, MsgEventInfo*& msgEventInfo) const
{
    std::streambuf* buffer = istr.rdbuf();
    TokenStream input(buffer);
    Token token(TT_error, "");

    input >> token;
    Terrain* terrain;
    if (!parse(input, terrain))
	return false;

    input >> token;
    Map* map;
    if (!parse(input, map))
	return false;

    input >> token;
    Course* course;
    if (!parse(input, course))
	return false;

    input >> token;
    Event* event;
    if (!parse(input, event))
	return false;
    
    msgEventInfo = new MsgEventInfo(terrain, map, course, event);
    return true;
}

#if 0
int main(int argc, char* argv[])
{
    if (argc > 1)
	readFile(argv[1]);
    return 0;
}
#endif

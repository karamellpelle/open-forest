#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <string.h>
#include <stdio.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "common.h"
#include "mesh.h"
#include "course.h"
#include "util.h"


Course::Course(std::string id, std::string name, std::string terrainId)
    : _id(id), _name(name), _terrainId(terrainId)
{
}

Course::Course(std::string name, std::string terrainId, int width, int height, int nControls)
    : _name(name), _terrainId(terrainId)
{
    realize(nControls, width, height);
    _id = genId();
}

Course::Course(std::string terrainId, int width, int height, int nControls) 
    : _name("H21A"), _terrainId(terrainId)
{
    realize(nControls, width, height);
    _id = genId();
}

Course::~Course()
{
    std::for_each(controls.begin(), controls.end(), del_fun<Control>());
}

fptype Course::directionToNextControl(const int controlNumber) const
{
    Control* thisControl = controls.at(controlNumber);
    Control* nextControl = controls.at(controlNumber+1);
    return atan2(nextControl->position.y - thisControl->position.y,
		 nextControl->position.x - thisControl->position.x) - PI/2;
}

Point Course::controlPosition(const int controlNumber) const
{
    Control* control = controls.at(controlNumber);
    return control->position;
}

std::string Course::genId() const
{
    std::ostringstream ostr;
    
    ostr << _terrainId;
    std::for_each(controls.begin(), controls.end(), generateID<Control*>(ostr));

    return std::string(md5sum(ostr.str().c_str()));
}

bool Course::operator==(const Course& other) const
{
    return _id == other._id;
}

Control* Course::controlAt(const int controlNumber) const
{
    return controls.at(controlNumber);
}

Control* Course::getControl(int number)
{
    Control* control = 0;
    try
    {
	control = controls.at(number);
    }
    catch (std::out_of_range&)
    {
    }
    return control;
}

static void draw0(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(1, 0);
    glVertex2i(1, 2);
    glVertex2i(0, 2);
    glVertex2i(0, 0);
    glEnd();
}

static void draw1(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(1, 2);
    glVertex2i(1, 0);
    glEnd();
}

static void draw2(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 2);
    glVertex2i(1, 2);
    glVertex2i(1, 1);
    glVertex2i(0, 1);
    glVertex2i(0, 0);
    glVertex2i(1, 0);
    glEnd();
}

static void draw3(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 2);
    glVertex2i(1, 2);
    glVertex2i(1, 1);
    glVertex2i(0, 1);
    glVertex2i(1, 1);
    glVertex2i(1, 0);
    glVertex2i(0, 0);
    glEnd();
}

static void draw4(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 2);
    glVertex2i(0, 1);
    glVertex2i(1, 1);
    glVertex2i(1, 2);
    glVertex2i(1, 0);
    glEnd();
}

static void draw5(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(1, 2);
    glVertex2i(0, 2);
    glVertex2i(0, 1);
    glVertex2i(1, 1);
    glVertex2i(1, 0);
    glVertex2i(0, 0);
    glEnd();
}

static void draw6(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(1, 2);
    glVertex2i(0, 2);
    glVertex2i(0, 0);
    glVertex2i(1, 0);
    glVertex2i(1, 1);
    glVertex2i(0, 1);
    glEnd();
}

static void draw7(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 2);
    glVertex2i(1, 2);
    glVertex2i(1, 0);
    glEnd();
}

static void draw8(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 1);
    glVertex2i(0, 2);
    glVertex2i(1, 2);
    glVertex2i(1, 0);
    glVertex2i(0, 0);
    glVertex2i(0, 1);
    glVertex2i(1, 1);
    glEnd();
}

static void draw9(void)
{
    glBegin(GL_LINE_STRIP);
    glVertex2i(0, 0);
    glVertex2i(1, 0);
    glVertex2i(1, 2);
    glVertex2i(0, 2);
    glVertex2i(0, 1);
    glVertex2i(1, 1);
    glEnd();
}

static void printDigit(int digit)
{
    switch (digit) {
    case 0: draw0(); break;
    case 1: draw1(); break;
    case 2: draw2(); break;
    case 3: draw3(); break;
    case 4: draw4(); break;
    case 5: draw5(); break;
    case 6: draw6(); break;
    case 7: draw7(); break;
    case 8: draw8(); break;
    case 9: draw9(); break;
    }
}

static void printNumber(GLfloat x, GLfloat y, int size, int number)
{
    char buffer[16];
    int len, i;

    sprintf(buffer, "%d", number);
    len = strlen(buffer);

    for (i = 0; i < len; i++) 
    {
	glPushMatrix();
	glTranslatef(x + i*2, y, 0);
	glScalef(size, size, size);
	printDigit(buffer[i]-'0');
	glPopMatrix();
    }
}

void Course::draw(const int nextControlNumber) const
{
    fptype angle;
    int circleRadius = 4;
    int numberSize = 3;
    int numberDistance = 10;

    glColor3f(1.0, 0.0, 1.0);

    int size = controls.size();
    for (int i = 0; i < size; i++)
    {
	bool firstControl = (i == 0);
	bool lastControl = (i == size-1);

	Control* thisControl = controls.at(i);

	if (!lastControl)
	{
	    // draw a line between controls
	    Control* nextControl = controls.at(i+1);
	    float xdiff = nextControl->position.x - thisControl->position.x;
	    float ydiff = nextControl->position.y - thisControl->position.y;
	    angle = atan2(ydiff, xdiff);
	    if (sqrt(xdiff*xdiff + ydiff*ydiff) > 2*circleRadius)
	    {
		// draw a line between controls only if they are 
		// not too close to each other
		glBegin(GL_LINES);
		glVertex2f(thisControl->position.x + (circleRadius+1)*cos(angle), 
			   thisControl->position.y + (circleRadius+1)*sin(angle));
		glVertex2f(nextControl->position.x - (circleRadius+1)*cos(angle), 
			   nextControl->position.y - (circleRadius+1)*sin(angle));
		glEnd();
	    }
	}

	if (firstControl)
	{
	    // this is the first control, i.e. it is actually 
	    // the start of the course, draw it as a triangle
	    glBegin(GL_LINE_LOOP);
	    glVertex2f(thisControl->position.x + (circleRadius+1)*cos(angle),
		       thisControl->position.y + (circleRadius+1)*sin(angle));
	    angle += 2*PI/3;
	    glVertex2f(thisControl->position.x + (circleRadius+1)*cos(angle),
		       thisControl->position.y + (circleRadius+1)*sin(angle));
	    angle += 2*PI/3;
	    glVertex2f(thisControl->position.x + (circleRadius+1)*cos(angle),
		       thisControl->position.y + (circleRadius+1)*sin(angle));
	    glEnd();
	}
	else
	{
	    // proper controls are drawn as a circle
	    if (nextControlNumber == i) 
	    {
		// the next control to catch is drawn in different color
		glColor3f(1.0, 0.0, 0.0);
	    }

	    glBegin(GL_LINE_LOOP);
	    for (angle = 0; angle < 2*PI; angle += 0.2) {
		glVertex2f(thisControl->position.x + circleRadius*cos(angle),
			   thisControl->position.y + circleRadius*sin(angle));
	    }
	    glEnd();

	    // draw number of this control
	    Control* prevControl = controls.at(i-1);
	    angle = atan2(thisControl->position.y - prevControl->position.y,
			  thisControl->position.x - prevControl->position.x);
	    if (!lastControl)
	    {
		Control* nextControl = controls.at(i+1);
		angle += atan2(thisControl->position.y - nextControl->position.y,
			       thisControl->position.x - nextControl->position.x);
	    }
	    angle /= 2;

	    glPushAttrib(GL_LINE_BIT);
	    glLineWidth(2.0);
	    printNumber(thisControl->position.x + numberDistance*cos(angle),
			thisControl->position.y + numberDistance*sin(angle),
			numberSize, i);
	    glPopAttrib();

	    if (nextControlNumber == i) 
	    {
		// reset the color
		glColor3f(1.0, 0.0, 1.0);
	    }
	}
    }

#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

void Course::realize(int numControls, int width, int height)
{
    Control* thisControl;
    Control* prevControl;
    float x, y, z = 0;
    int i;
    fptype angleConstraint, lengthConstraint;
    fptype rndAngle, rndLength, angle;
    fptype courseLength = 0;

    int seed = time((time_t*)0);
    myrandomSeed(seed % 900000000);

    /* We try to position the controls in a sensible way
     * so that the lines between controls do not cross each
     * other too much. This is achieved so that the direction
     * to the next control is determined from the line from
     * the previous control to this control. When the angle 
     * between these lines is not too small, we can avoid 
     * most crossing lines. 
     */
    for (i = 0; i < 2; i++) 
    {
	/* the start and first controls need not be restricted */
	x = myrandom(5, width-5);
	y = myrandom(5, height-5);
	thisControl = new Control(x, y, z);
	if (i == 0) 
	{
	    prevControl = thisControl;
	}
	else 
	{
	    courseLength += prevControl->position.distance(thisControl->position);
	}
	addControl(thisControl);

    }
    for (i = 0; i < numControls-2; i++) 
    {
	/* the subsequent control positions do need restrictions 
	* although we relax them if we can't find good positions
	* otherwise */
	angle = atan2(thisControl->position.y - prevControl->position.y,
		      thisControl->position.x - prevControl->position.x);

	angleConstraint = PI;
	lengthConstraint = std::max(width-20, height-20);

	while (true)
	{
	    /* repeat so long that we find an acceptable position
	     * for this control, relaxing restrictions in every
	     * iteration */
	    rndAngle = (rand()/(fptype)RAND_MAX)*angleConstraint -
		(angleConstraint/2);
	    rndLength = (rand()/(fptype)RAND_MAX)*lengthConstraint + 20;
	    x = thisControl->position.x + rndLength*cos(angle+rndAngle);
	    y = thisControl->position.y + rndLength*sin(angle+rndAngle);

	    if (x >= 5 && x < width-5 && y >= 5 && y < height-5)
		break; /* we've got an acceptable position */
	    angleConstraint += PI/16;
	    lengthConstraint -= 10;
	    lengthConstraint = std::max(lengthConstraint, 10.0f);
	}

	prevControl = thisControl;
	thisControl = new Control(x, y, z);
	courseLength += prevControl->position.distance(thisControl->position);

	addControl(thisControl);
    }
}

void Course::reset()
{
    std::for_each(controls.begin(), controls.end(), std::mem_fun<int, Control>(&Control::reset));
}

void Course::addControl(Control* control)
{
    controls.push_back(control);
}

std::ostream& Course::toXML(std::ostream& ostr, int indent) const
{
    std::string i(indent, ' ');

    ostr << i << "<course>\n";
    ostr << i << "  <id>" << _id << "</id>\n";
    ostr << i << "  <name>"<< _name << "</name>\n";
    ostr << i << "  <terrainId>" << _terrainId << "</terrainId>\n";
    ostr << i << "  <controls>\n";

    std::for_each(controls.begin(), controls.end(), printXML<Control*>(ostr, indent+4));

    ostr << i << "  </controls>\n";
    ostr << i << "</course>\n";

    return ostr;
}

std::string Course::toString() const
{
    std::ostringstream ostr;
    ostr << _name << " " << controls.size();
    return ostr.str();
}


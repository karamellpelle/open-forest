#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define GLEW_STATIC 1
#include <GL/glew.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "common.h"
#include "matrix.h"
#include "point.h"
#include "control.h"
#include "orienteer.h"
#include "run.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"
#include "course.h"
#include "conditions.h"
#include "database.h"
#include "observer.h"
#include "eventproxy.h"
#include "oriproxy.h"
#include "mainWindow.h"
#include "shader.h"

using std::max;
using std::min;

//static float diffuse[] = {1.0, 1.0, 1.0, 1.0};
//static float specular[] = {0.0, 0.0, 0.0, 1.0};
//static float ambient[] = {0.3, 0.3, 0.3, 1.0};

const fptype OrienteerProxy::orienteerHeight = 1.0;

Orienteer* OrienteerProxy::getOrienteer()
{
    return orienteer;
}

/* Calculate new position for the orienteer.
 */
void OrienteerProxy::newPosition()
{
    setPosition(location->position.x + stepLength*dircos,
		location->position.y + stepLength*dirsin);
}

/* reset orienteer proxy to the start of course with nose
 * pointing to the direction of first control */
void OrienteerProxy::reset(int id)
{
    Course* course = Database::instance().getEventCourse();

    nextControlNumber = 0;

    Control* start = course->getControl(0);

    nextControl = course->getControl(1);

    location->horizontal =
    location->runDirection =
	atan2(nextControl->position.y - start->position.y,
	      nextControl->position.x - start->position.x) - PI/2;
    location->vertical = PI/2;

    int row = id/3;
    int column = (id%3) - 1; // value is -1, 0, 1
    int distanceBetweenPlayers = 1;

    fptype dir = location->horizontal + PI;
    fptype x = start->position.x + row*distanceBetweenPlayers*cos(dir);
    fptype y = start->position.y + row*distanceBetweenPlayers*sin(dir);
    //dir += PI/2;
    x += column*distanceBetweenPlayers*cos(dir);
    y += column*distanceBetweenPlayers*sin(dir);
    
    TerrainProxy& terrainProxy = TerrainProxy::instance();
    mesh = terrainProxy.getMesh();

    terrainWidth = mesh->getWidth();
    terrainHeight = mesh->getHeight();

    setPosition(x, y);
    course->reset();
    prevPosition.x = location->position.x;
    prevPosition.y = location->position.y;
    stepLength = 0.0;
#if 0
	// stationary light
	GLfloat model[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		-location->position.x, -location->position.y, -location->position.z, 1.0
	};
	setUniformMatrix4fv("WCtoLC", 1, model);
#elif 1
	int xx = -location->position.x;
	int yy = -location->position.y;
	int zz = -location->position.z-1.0;
	model[ 0] = 1.0; model[ 1] = 0.0; model[ 2] = 0.0; model[ 3] = 0.0;
	model[ 4] = 0.0; model[ 5] = 1.0; model[ 6] = 0.0; model[ 7] = 0.0;
	model[ 8] = 0.0; model[ 9] = 0.0; model[10] = 1.0; model[11] = 0.0;
	model[12] =  xx; model[13] =  yy; model[14] =  zz; model[15] = 1.0;
#endif
}

void OrienteerProxy::start()
{
    Database& db = Database::instance();
    Event* event = db.getSelectedEvent();
    orienteer = db.getSelectedOrienteer();
    if (run != NULL)
	delete run;
    run = new Run("test", event->id(), orienteer->id());
    run->recordPosition(&location->position);
    run->setStatus(Run::OK);
    prevRecordedPosition.x = location->position.x;
    prevRecordedPosition.y = location->position.y;

    ++nextControlNumber;

    run->start();
}

void OrienteerProxy::finish()
{
    run->recordPosition(&location->position);
    run->finish();
}

void OrienteerProxy::quit()
{
    run->setStatus(Run::DNF);
    finish();
}

fptype OrienteerProxy::getTotalTime() const
{
    return run->getTotalTime();
}

OrienteerProxy::OrienteerProxy()
{
    location = new Location(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    stepLength = 0;
    prevRecordedPosition.x = location->position.x;
    prevRecordedPosition.y = location->position.y;
    opObserver = NULL;
	for (int i = 0; i < maxLights; i++) {
		pos[i*3+0] = pos[i*3+1] = pos[i*3+2] = 0.0;
		hor[i] = 0.0;  
		ver[i] = 0.0;  
	}
}

OrienteerProxy::~OrienteerProxy()
{
    delete location;
}

void OrienteerProxy::disqualify()
{
    if (run != NULL) 
    {
	run->setStatus(Run::DQF);
    }
}

Run::RunStatus OrienteerProxy::getStatus() const
{
    return run->getStatus();
}

// return true if race is finished, i.e. this is the last control
bool OrienteerProxy::punch()
{
    Course* course = Database::instance().getEventCourse();
    Control* prevControl = course->getControl(nextControlNumber-1);
    run->recordPosition(&location->position);
    prevRecordedPosition.x = location->position.x;
    prevRecordedPosition.y = location->position.y;

    run->punch(prevControl, nextControl, run->trackLength());
    nextControl = course->getControl(++nextControlNumber);

    return (nextControl == NULL);
}

/* Draw this orienteer to map. This consists of small dot
 * showing the location and a line showing the direction: O-
 */
void OrienteerProxy::drawToMap(bool drawOthers)
{
    Point pos = getPosition();
    fptype angle = getRunDirection();

    glPointSize(3.0);
    glColor3f(1.0, 0.0, 0.0); // red

    glPushMatrix();

    // modeling transformation
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(Rad2Deg(angle), 0.0, 0.0, 1.0);

    // draw a point indicating orienteer's position
    glBegin(GL_POINTS);
    glVertex2f(0.0, 0.0);
    glEnd();

    // draw a line showing the direction of view
    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, 10.0);
    glEnd();

    glPopMatrix();

#if 0
    eventProxy->drawToMap();
#endif
#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

void OrienteerProxy::drawRunToMap()
{
    run->draw(false);
}

#if 0
void opLightUpdate(OrienteerProxy self, int light, char on)
{
    twMakeCurrent(self->terrainWin);
    if (on) 
    {
	glLightfv((GLenum)light, GL_DIFFUSE, diffuse);
	glLightfv((GLenum)light, GL_SPECULAR, specular);
	glLightfv((GLenum)light, GL_AMBIENT, ambient);
	glLightf((GLenum)light, GL_SPOT_CUTOFF, 20.0);
	glLightf((GLenum)light, GL_LINEAR_ATTENUATION, 0.1);
	glLightf((GLenum)light, GL_QUADRATIC_ATTENUATION, 0.05);

	glEnable((GLenum)light);
    } 
    else 
    {
	glDisable((GLenum)light);
    }

    //checkGLError();
}
#endif

bool OrienteerProxy::runInProgress()
{
    return run != NULL;
}

void OrienteerProxy::setPosition(fptype x, fptype y)
{
    const fptype one = 1;

    location->position.x = min(max(one, x), fptype(terrainWidth-1));
    location->position.y = min(max(one, y), fptype(terrainHeight-1));

    location->position.z = mesh->calcZ(location->position.x, location->position.y) + orienteerHeight;

    fptype dist = location->position.distance(prevRecordedPosition);
    if (dist > 5 && run != NULL) 
    {
	// we save only if we have run far enough from the previous recorded position
	run->recordPosition(&location->position);
        prevRecordedPosition.x = location->position.x;
        prevRecordedPosition.y = location->position.y;
    }
}

Point OrienteerProxy::getPosition()
{
    return location->position;
}

fptype OrienteerProxy::getVertical()
{
    return location->vertical;
}

fptype OrienteerProxy::getRunDirection()
{
    return location->runDirection;
}

Location* OrienteerProxy::getLocation()
{
    return location;
}

Control* OrienteerProxy::getNextControl()
{
    return nextControl;
}

int OrienteerProxy::getNextControlNumber()
{
    return nextControlNumber;
}

Run* OrienteerProxy::getRun()
{
    return run;
}

void OrienteerProxy::addDirection(fptype toAdd, bool standingStill, bool lookingAround)
{
    if (!standingStill && !lookingAround) 
    {
	location->runDirection += toAdd;
    }
    else if (standingStill && lookingAround)
    {
	location->runDirection += toAdd;
	location->horizontal += toAdd;
    }
    else if (!standingStill && lookingAround)
    {
	location->horizontal += toAdd;
	// limit the range you can turn your head
	// to 90 degrees, or PI/2 radians to left or right
	// from the running direction

	location->horizontal = min(max(location->horizontal, 
				       float(location->runDirection-PI/2)),
				   float(location->runDirection+PI/2));
    }
}

#define VERTICAL_TRESHOLD PI/8

void OrienteerProxy::addVertical(fptype toAdd)
{
    location->vertical += toAdd;
    location->vertical = min(max(float(location->vertical), float(VERTICAL_TRESHOLD)), 
			     float(PI-VERTICAL_TRESHOLD));
}

// Draw compass, red end points north
void OrienteerProxy::drawCompass(int winWidth, int winHeight)
{
    GLfloat angle = Rad2Deg(location->runDirection);
    GLfloat compassSize = 30.0;

    glPushMatrix();
    glLoadIdentity();

    glTranslatef(winWidth/2 - compassSize, winHeight/2 - compassSize, 0.5);
    glRotatef(-angle, 0.0, 0.0, 1.0);
    glScalef(compassSize, compassSize, 0.0);

    glBegin(GL_QUAD_STRIP);

    glColor3f(1.0, 0.0, 0.0); /* red */
    glVertex2f(0.1, 1.0);
    glVertex2f(-0.1, 1.0);
    glVertex2f(0.1, 0.0);
    glVertex2f(-0.1, 0.0);

    glColor3f(0.0, 0.0, 0.0); /* black */
    glVertex2f(0.1, -1.0);
    glVertex2f(-0.1, -1.0);

    glEnd();

    glPopMatrix();

#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

static fptype dotProduct2f(fptype vec1[], fptype vec2[])
{
    return vec1[0]*vec2[0] + vec1[1]*vec2[1];
}

/* Return the target speed for orienteer.
 * This is a function of terrain steepness,
 * uphill is slower to run than downhill, thus 
 * target speed on an uphill is smaller that on a downhill.
 * As side effect, also adjust the direction orienteer is facing.
 */ 
fptype OrienteerProxy::getTargetSpeed()
{
    fptype* normal = mesh->getFacetNormal(location->position.x, location->position.y);
    fptype vec1[2];
    fptype vec2[2];
    fptype vec3[2];
    fptype slopeAhead;
    fptype slopeRight;

    // project facet normal to x,y-plane by ignoring z coord
    // this is the direction and steepness of downhill (gradient?)
    vec1[0] = normal[0];
    vec1[1] = normal[1];

    // direction where the orienteer is looking at
    // for determining whether we are facing uphill/downhill
    vec2[0] = dircos = cos(location->runDirection + PI/2);
    vec2[1] = dirsin = sin(location->runDirection + PI/2);

    // left/right direction of the orienteer
    // for determining whether downhill is to your left/right
    vec3[0] = cos(location->runDirection);
    vec3[1] = sin(location->runDirection);

    // This is the slope of the terrain in the direction we are facing.
    // Value is between -1 and 1. 
    // Values <0 means we are facing uphill
    // Values >0 means we are facing downhill
    slopeAhead = dotProduct2f(normal, vec2);

    // this is the slope of the terrain in left/right direction
    // Values <0 means downhill is to the left
    // Values >0 means downhill is to the right
    slopeRight = dotProduct2f(normal, vec3);

    // apply the change to our direction
    // this implements the feature that our direction tends to change 
    // to the direction of downhill because gravity pulls us down
    location->runDirection -= slopeRight/60;

    return slopeAhead*3 + 3; // this scales and translates it between 0 and 6
}

/* Requirements for the movement of the orienteer:
 * 1) Orienteer runs at the same speed on different computers with different specs
 * 2) Uphill is slower to run than downhill
 * 3) When running on a slope, your direction tends to change so that you turn towards downhill,
 *    because gravity pulls you down
 * 
 * We want to be able to calculate the directional derivative (?) relative to our
 * current location and (horizontal) direction. The result would be two floats,
 *   (1) the derivative on the direction we are facing (i.e. whether we are
 *   facing uphill or downhill). This would be
 *     =0 for flat surface
 *     <0 for uphill
 *     >0 for downhill
 *   (2) the direction perpendicular to (1) telling whether downhill is to our 
 *   left or right (i.e. which way our horizontal direction tends to change)
 *     =0 for flat surface
 *     <0 when dowhill is to our left
 *     >0 when dowhill is to our right
 *
 * There is a target speed that is the same for all computers.
 * This target speed is depedent on the slope of the hill, uphill is slower than
 * downhill. For example (all units here meters / second):
 *   3 m/s on a flat surface
 *   6 m/s on downhill
 *   1 m/s on a steep uphill
 * So the target speed is a function of directional derivative (1). The step
 * length is constantly adjusted so that our current speed approaches target speed.
 * The change in our horizontal direction is a function of derivative (2).
 *
 */

/*
 * Adjust step length so that our speed converges towards target speed.
 * Also calculate frame rate (frames per second, or fps)
 */
void OrienteerProxy::adjustStepLength(bool standingStill)
{
    static GLint time0 = 0;
    static GLint frames = 0;
    GLint time;
    int frequency = 200; // do calculations once per this many milliseconds

    frames++;

    // calculate our speed and adjust step length accordingly
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - time0 > frequency) 
    {
	float seconds = (time - time0) / 1000.0;
	fptype distance = location->position.distance(prevPosition);
	float currentSpeed = distance / seconds;
	float targetSpeed = standingStill ? 0 : getTargetSpeed();
	float minStepLength = 0.0;
	float maxStepLength = 6.0;

	/* Determines how fast current speed converges towards target speed.
	 * Bigger values mean slower convergence. */
	int speedConvergence = 300;
	MainWindow::framesPerSecond = frames / seconds;

	/* steplength:
	 *   gets bigger  if current speed is below target speed
	 *   gets smaller if current speed is above target speed
	 */
	stepLength += (targetSpeed-currentSpeed) / speedConvergence;
	stepLength = min(max(minStepLength, stepLength), maxStepLength);

#if 0
	printf("Frame rate: %f\n", framesPerSecond);
	printf("Distance: %f\n", distance);
	printf("currentSpeed: %f\n", currentSpeed);
	printf("targetSpeed: %f\n", targetSpeed);
	printf("stepLength: %f\n", stepLength);
#endif

	time0 = time;
	prevPosition.x = location->position.x;
	prevPosition.y = location->position.y;

	frames = 0;
    }
}

// if not looking around, turn horizontal direction to match the run direction
void OrienteerProxy::adjustHorizontal(bool lookingAround)
{
    if (!lookingAround)
    {
	fptype diff = location->runDirection - location->horizontal;
	location->horizontal += diff/10;
    }
}

void OrienteerProxy::resetRunDirection()
{
    location->runDirection = location->horizontal;
}

/******************************/


void OrienteerProxy::view()
{
    glLoadIdentity();

    // viewing transformation
    ver[0] = -(location->vertical);
    hor[0] = -(location->horizontal);
    GLfloat x = -location->position.x;
    GLfloat y = -location->position.y;
    GLfloat z = -location->position.z;
    pos[0] = x; pos[1] = y; pos[2] = z;
    glRotatef(-Rad2Deg(location->vertical), 1.0, 0.0, 0.0);
    glRotatef(-Rad2Deg(location->horizontal), 0.0, 0.0, 1.0);
    glTranslatef(x, y, z);

    // draw terrain and other orienteers
    eventProxy->draw(location);

    // draw next control
    if (nextControl != NULL) 
    {
	fptype distance = location->position.distance(nextControl->position);
	if (distance < controlViewTreshold)
	{
	    nextControl->draw();
	    if (distance < controlStampTreshold) 
	    {
		if (opObserver != NULL)
		    opObserver->punch(nextControlNumber);
	    }
	}
    }

#ifdef CHECK_GL_ERROR
   checkGLError();
#endif
}

void OrienteerProxy::viewDay()
{
    /* sun is a directional light, which means all rays are parallel.
     * In OpenGL this is determined by the last argument here
     * that has to be zero for directional light.
     */
    GLfloat sun_position[] = {-10.0, -10.0, 10.0, 0.0};

    /* headlight is positional light (last arg = 1.0)
     * position 0.0, 0.0, 0.0 makes it shine out of camera lens
     * direction 0.0, 0.0, -1.0 makes it shine in the direction of our view
     */
    GLfloat headlight_position[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat headlight_direction[] = {0.0, 0.0, -1.0};

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    /* draw headlight
     * we want headlight to move with the camera
     * it has to be drawn in eye coordinates, i. e. before any 
     * viewing transformations */
    if (night) 
    {
	glLightfv(light, GL_POSITION, headlight_position);
	glLightfv(light, GL_SPOT_DIRECTION, headlight_direction);
    }

    // viewing transformation
    ver[0] = -(location->vertical);
    hor[0] = -(location->horizontal);
    GLfloat x = -location->position.x;
    GLfloat y = -location->position.y;
    GLfloat z = -location->position.z;
    pos[0] = x; pos[1] = y; pos[2] = z;
    glRotatef(-Rad2Deg(location->vertical), 1.0, 0.0, 0.0);
    glRotatef(-Rad2Deg(location->horizontal), 0.0, 0.0, 1.0);
    glTranslatef(x, y, z);

    /* draw sunlight
     * sunlight is stationary light
     * it has to be drawn in world coordinates before any 
     * modeling transformations */ 
    if (!night) 
    {
	glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
    }

    // draw terrain and other orienteers
    eventProxy->drawDay(location);

    // draw next control
    if (nextControl != NULL) 
    {
	fptype distance = location->position.distance(nextControl->position);
	if (distance < controlViewTreshold)
	{
	    nextControl->draw();
	    if (distance < controlStampTreshold) 
	    {
		if (opObserver != NULL)
		    opObserver->punch(nextControlNumber);
	    }
	}
    }
#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

void OrienteerProxy::viewNight()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // viewing transformation
    ver[0] = -(location->vertical);
    hor[0] = -(location->horizontal);
    GLfloat x = -location->position.x;
    GLfloat y = -location->position.y;
    GLfloat z = -location->position.z;
    pos[0] = x; pos[1] = y; pos[2] = z;
    
    glRotatef(-Rad2Deg(location->vertical), 1.0, 0.0, 0.0);
    glRotatef(-Rad2Deg(location->horizontal), 0.0, 0.0, 1.0);
    glTranslatef(x, y, z);
    
    int nextIndex = eventProxy->getCoordinates(pos, hor, ver, 1);
    nextIndex = maxLights;
    //std::cout << nextIndex << "\n";
    //setUniform1i(nightShader, "numEnabledLights", nextIndex);
    setUniform1fv(nightShader, "horizontal", nextIndex, hor);
    setUniform1fv(nightShader, "vertical", nextIndex, ver);
    setUniform3fv(nightShader, "position", nextIndex, pos);

    setUniform1fv(nightTrees, "horizontal", nextIndex, hor);
    setUniform1fv(nightTrees, "vertical", nextIndex, ver);
    setUniform3fv(nightTrees, "position", nextIndex, pos);

    // draw terrain and other orienteers
    eventProxy->drawNight(location);

	// draw next control
    if (nextControl != NULL) 
    {
	fptype distance = location->position.distance(nextControl->position);
	if (distance < controlViewTreshold)
	{
	    nextControl->draw();
	    if (distance < controlStampTreshold) 
	    {
		if (opObserver != NULL)
		    opObserver->punch(nextControlNumber);
	    }
	}
    }
}

void OrienteerProxy::setStepLength(fptype sl)
{
    stepLength = sl;
}

void OrienteerProxy::setOpObserver(OpObserver* op_observer)
{
    opObserver = op_observer;
}


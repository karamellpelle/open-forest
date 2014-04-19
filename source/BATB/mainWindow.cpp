#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#define GLEW_STATIC 1
#include <GL/glew.h>

#ifdef FREEGLUT_IS_PRESENT
#  include <GL/freeglut.h>
#else
#  ifdef __APPLE__
#    include <GLUT/glut.h>
#  else
#    include <GL/glut.h>
#  endif
#endif

#include <plib/pu.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "common.h"

#include "database.h"
#include "xmlParser.h"

#include "matrix.h"
#include "observer.h"
#include "conditions.h"
#include "terrainproxy.h"
#include "mapproxy.h"
#include "eventproxy.h"
#include "oriproxy.h"

#include "dlgCommon.h"
#include "dlgEvent.h"
#include "dlgResult.h"
#include "dlgPlayers.h"
#include "dlgSingle.h"
#include "dlgMenu.h"
#include "dlgStack.h"
#include "dlgJoin1.h"
#include "dlgJoin2.h"
#include "dlgHost1.h"
#include "dlgHost2.h"
#include "windowMessage.h"
#include "mainWindow.h"
#include "texture.h"
#include "fdManager.h"
#include "server.h"
#include "shader.h"
#include "noise.h"
//#include "forest.h"

using batb::XmlParser;


GLuint treeTextures[4];
GLuint groundTextures[4];
GLuint altgradTextures[4];
GLuint noiseTextures[4];

GLfloat light_diffuse [] = {0.0, 1.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */

MainWindow mainWindow;

int     MainWindow::main_window;
puText* MainWindow::fps_text;
bool MainWindow::showMap;
fptype MainWindow::mapScale; 
fptype MainWindow::mapScaleMin;
fptype MainWindow::mapScaleMax;
fptype MainWindow::mapRotate; 
Point MainWindow::mapEye; 
Point MainWindow::thumb; 
Point MainWindow::mouse;
char MainWindow::mouseButton;
int MainWindow::winPosX;
int MainWindow::winPosY;
int MainWindow::winWidth;
int MainWindow::winHeight;
int MainWindow::oldWinWidth;
int MainWindow::oldWinHeight;
bool MainWindow::standingStill;
bool MainWindow::lookingAround;
bool MainWindow::movingMap;
bool MainWindow::rotatingMap;
bool MainWindow::zoomingMap;
bool MainWindow::gameOn;
bool MainWindow::typingMessage;
bool MainWindow::showDialogs;
bool MainWindow::showFps;
bool MainWindow::fullScreen;
bool MainWindow::showLocation;
bool MainWindow::showRoute;
bool MainWindow::showRoutes;
bool MainWindow::settingLocation;

Course* MainWindow::course;
float MainWindow::framesPerSecond;
char MainWindow::fpsTxt[32];
WindowMessages* MainWindow::windowMessages;
FdManager MainWindow::fdManager;
Server* MainWindow::server;
Client* MainWindow::client;
DlgProgress* MainWindow::dlgProgress;
MainWindow::continuationFunction MainWindow::continuationFunc;

GLuint dayShader;   // shader for day
GLuint nightShader; // shader for night
GLuint nightTrees; // shader for trees at night

MainWindow::DisplayFunc MainWindow::terrainDF;
MainWindow::DisplayFunc MainWindow::mapDF;


static void initTextures()
{
    glGenTextures(4, treeTextures);
    glGenTextures(4, groundTextures);
    glGenTextures(4, altgradTextures);
    glGenTextures(4, noiseTextures);

    glActiveTexture(GL_TEXTURE0);
    // for each tree type assign: min and max height and width,
    // occurrence frequency (probability it will be chosen)
#if 1
    loadTexture(treeTextures[0], "images/manty.rgb");
    loadTexture(treeTextures[1], "images/kuusi.rgb");
    loadTexture(treeTextures[2], "images/rauduskoivu.rgb");
    loadTexture(treeTextures[3], "images/pihlaja.rgb");
#else
    Forest forest;
    forest.addTreeType(20, 35, 10, 15, 10,
		       treeTextures[0], "images/manty.rgb");
    forest.addTreeType(20, 35, 10, 15, 10,
		       treeTextures[1], "images/kuusi.rgb");
    forest.addTreeType(10, 25, 10, 15,  8,
		       treeTextures[2], "images/rauduskoivu.rgb");
    forest.addTreeType( 5, 15, 10, 15,  5,
			treeTextures[3], "images/pihlaja.rgb");
#endif
#if 0
    loadTexture(treeTextures[4], "images/douglas.rgb");
    loadTexture(treeTextures[5], "images/haapa.rgb");
    loadTexture(treeTextures[6], "images/harmaaleppa.rgb");
    loadTexture(treeTextures[7], "images/hieskoivu.rgb");
    loadTexture(treeTextures[8], "images/jalava.rgb");
    loadTexture(treeTextures[9], "images/kataja.rgb");
    loadTexture(treeTextures[10], "images/lehmus.rgb");
    loadTexture(treeTextures[11], "images/lehtikuusi.rgb");
    loadTexture(treeTextures[12], "images/pihta.rgb");
    loadTexture(treeTextures[13], "images/raita.rgb");
    loadTexture(treeTextures[14], "images/saarni.rgb");
    loadTexture(treeTextures[15], "images/tammi.rgb");
    loadTexture(treeTextures[16], "images/tervaleppa.rgb");
    loadTexture(treeTextures[17], "images/tuomi.rgb");
    loadTexture(treeTextures[18], "images/vaahtera.rgb");
#endif

    glActiveTexture(GL_TEXTURE1);
    loadTexture(altgradTextures[0], "images/altgrad.rgb");

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, noiseTextures[0]);
    createNoise3D();

    glActiveTexture(GL_TEXTURE0);
}

void MainWindow::projection()
{
    glViewport(0, 0, (GLint)winWidth, (GLint)winHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (showMap) 
    {
	// this projection puts the coordinate origin in the center of the window
	gluOrtho2D(-(winWidth/2),  (winWidth/2),
		   -(winHeight/2), (winHeight/2));
    }
    else 
    {
	gluPerspective(60, 1.0, 0.1, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);

#ifdef CHECK_GL_ERROR
    checkGLError();
#endif
}

void MainWindow::displayMap()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_FLAT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    setDisplayFunc(MainWindow::mapDF);
}

void MainWindow::displayTerrain()
{
    Database& db = Database::instance();
    Event* event = db.getSelectedEvent();
    if (event->night()) 
        glClearColor(0.0, 0.0, 0.0, 1.0);
    else
        glClearColor(0.0, 0.746, 1.0, 1.0);

    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
	
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    setDisplayFunc(MainWindow::terrainDF);
}

void MainWindow::displaySwitch()
{
    if (showMap) 
    {
	MainWindow::displayMap();
    } 
    else
    {
	MainWindow::displayTerrain();
    }
    MainWindow::projection();
}

/**************************************\
 *                                      *
 * These three functions capture mouse  *
 * and keystrokes (special and mundane) *
 * from GLUT and pass them on to PUI.   *
 *                                      *
\**************************************/

void MainWindow::specialDownFn(int key, int, int)
{
    puKeyboard(key + PU_KEY_GLUT_SPECIAL_OFFSET, PU_DOWN);
    glutPostRedisplay();
    printf("special key down: %c\n", key);
}

void MainWindow::specialUpFn(int key, int, int)
{
    puKeyboard(key + PU_KEY_GLUT_SPECIAL_OFFSET, PU_UP);
    glutPostRedisplay();
    printf("special key up: %c\n", key);
}

void MainWindow::keyDownFn(unsigned char key, int, int)
{
    puKeyboard(key, PU_DOWN);

    if (typingMessage)
    {
	switch (key)
	{
	case 27: // esc 
	    typingMessage = false;
	    windowMessages->hideInput();
	    break;
	case 13: // enter
	    typingMessage = false;
	    windowMessages->hideInput();
	    std::string msg = windowMessages->getTypedText();
	    windowMessages->addMessage(msg);
	    if (client != NULL)
		client->sendChat(msg);
	    break;
	}
    }
    else if (showDialogs)
    {
	switch (key)
	{
	case 27: // esc 
	    showDialogs = false;
	    DlgStack::instance().setVisible(showDialogs);
	    break;
	}
    }
    else
    {
	switch (key)
	{
	case 27: // esc toggles dialogs
	    showDialogs = true;
	    mouseButton = 0;
	    actionMapping(mouseButton);
	
	    DlgStack::instance().setVisible(showDialogs);
	    break;
	case ' ': // space toggles map
	    if (gameOn)
		showMap = !showMap;
	    MainWindow::displaySwitch();
	    break;
	case 'f':
	    showFps = !showFps;
	    if (showFps)
		fps_text->reveal();
	    else
		fps_text->hide();
	    break;
	case 't':
	    typingMessage = true;
	    windowMessages->clearInput();
	    windowMessages->showInput();
	    break;
	case 'l':
	    if (showMap)
	    {
		show_location_cb(NULL);
	    }
	    break;
	case 'r':
	    if (showMap)
	    {
		show_route_cb(NULL);
	    }
	    break;
	case 's':
	    if (showMap)
	    {
		settingLocation = !settingLocation;
		set_location_cb(NULL);
	    }
	    break;
	}
    }
    
    switch (key)
    {
    case 'u':
	fullScreen = !fullScreen;
	if (fullScreen)
	{
	    oldWinWidth = winWidth;
	    oldWinHeight = winHeight;
	    glutFullScreen();
	}
	else
	{
	    glutPositionWindow(winPosX, winPosY);
	    glutReshapeWindow(oldWinWidth, oldWinHeight);
	}
	break;
    }

    glutPostRedisplay();
}

void MainWindow::keyUpFn(unsigned char key, int, int)
{
    puKeyboard(key, PU_UP);
    printf("keyUp: %c\n", key);
}

void MainWindow::motionfn(int x, int y)
{
    if (showMap)
    {
	if (zoomingMap) 
	{
	    // move the mouse with both left and right button depressed, zoom the map
	    fptype deltay = mouse.y - y;
	    mapScale += deltay/30.0;
	    if (mapScale < mapScaleMin) mapScale = mapScaleMin;
	    if (mapScale > mapScaleMax) mapScale = mapScaleMax;
	} 
	else if (movingMap) 
	{
	    // moving the mouse with left button depressed, move the map
	    fptype deltax = mouse.x - x;
	    fptype deltay = mouse.y - y;
	    fptype angle = atan2(deltay, deltax);
	    fptype length = sqrt(deltax*deltax + deltay*deltay)/mapScale;
	    mapEye.x += length*cos(angle + mapRotate);
	    mapEye.y -= length*sin(angle + mapRotate);
	} 
	else if (rotatingMap) 
	{
	    // moving the mouse with right button depressed, rotate the map
	    int centerx = winWidth/2;
	    int centery = winHeight/2;
	    fptype deltaAngle = atan2(mouse.y-centery, mouse.x-centerx);
	    deltaAngle -= atan2(fptype(y-centery), fptype(x-centerx));
	    mapRotate += deltaAngle;
	}
    }
    else if (showDialogs)
    {
    }
    else
    {
	OrienteerProxy& op = OrienteerProxy::instance();
	op.addDirection(-((x - mouse.x)/(fptype)100), standingStill, lookingAround);
	op.addVertical(-((y - mouse.y)/(fptype)100));
    }

    mouse.x = x;
    mouse.y = y;
    puMouse(x, y);
    glutPostRedisplay();
}

void MainWindow::actionMapping(int mouseButton)
{
    if (showMap)
    {
	movingMap = bool((mouseButton & 1));
	rotatingMap = bool((mouseButton & 4));
	zoomingMap = bool((mouseButton & 1) && (mouseButton & 4));
	standingStill = true;
	lookingAround = false;
    }
    else if (!gameOn)
    {
	movingMap = false;
	rotatingMap = false;
	zoomingMap = false;
	standingStill = true;
	lookingAround = true;
    }
    else // gameOn
    {
	movingMap = false;
	rotatingMap = false;
	zoomingMap = false;
	standingStill = !(mouseButton & 1);
	lookingAround = (mouseButton & 4);
    }

#if 0
    std::cout << "movingMap " << movingMap << std::endl 
	      << "rotatingMap " << rotatingMap << std::endl 
	      << "zoomingMap " << zoomingMap << std::endl 
	      << "standingStill " << standingStill << std::endl 
	      << "lookingAround " << lookingAround << std::endl;
#endif
}

void MainWindow::unproject(fptype x, fptype y, fptype z,
			   fptype* objx, fptype* objy, fptype* objz)
{
    GLdouble model[16];
    GLdouble projection[16];
    GLint viewport[4];
    GLdouble ox, oy, oz;

    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble)z,
                 model, projection, viewport,
                 &ox, &oy, &oz);
    *objx = (fptype)ox;
    *objy = (fptype)oy;
    *objz = (fptype)oz;
}

void MainWindow::mousefn(int button, int updown, int x, int y)
{
    mouse.x = x;
    mouse.y = y;

    if (settingLocation)
    {
        fptype xx = (fptype)x;
        fptype yy = (fptype)winHeight-y;
        fptype zz = (fptype)1;
        fptype objx, objy, objz;
        unproject(xx, yy, zz, &objx, &objy, &objz);
        OrienteerProxy& oriProxy = OrienteerProxy::instance();
        oriProxy.setPosition(objx, objy);
        settingLocation = false;
        windowMessages->setMessage(WindowMessages::HA_CENTER,
                                   WindowMessages::VA_BOTTOM,
                                   "");
    }
    else if (showDialogs)
    {
        mouseButton = 0;
        actionMapping(mouseButton);
    }
    else
    {
	static int downx;
	static int downy;
        switch (updown) 
        {
        case GLUT_DOWN:
            switch (button) 
            {
            case GLUT_LEFT_BUTTON:
                mouseButton |= 1;
		downx = x;
		downy = y;
                break;
            case GLUT_MIDDLE_BUTTON:
                mouseButton |= 2;
                break;
            case GLUT_RIGHT_BUTTON:
                mouseButton |= 4;
                break;
            }
            break;
	    
	case GLUT_UP:
	    switch (button) 
	    {
	    case GLUT_LEFT_BUTTON:
		mouseButton &= ~1;
		if (showMap && downx == x && downy == y)
		{
		    fptype xx = (fptype)x;
		    fptype yy = (fptype)winHeight-y;
		    fptype zz = (fptype)1;
		    fptype objx, objy, objz;
		    unproject(xx, yy, zz, &objx, &objy, &objz);
		    thumb.x = objx;
		    thumb.y = objy;
		}
		break;
	    case GLUT_MIDDLE_BUTTON:
		mouseButton &= ~2;
	    break;
	    case GLUT_RIGHT_BUTTON:
		mouseButton &= ~4;
		break;
	    }
	}
	
	actionMapping(mouseButton);
    }

    puMouse(button, updown, x, y);
    glutPostRedisplay();
}

void MainWindow::checkSockets()
{
    fd_set readfds;
    int maxfd;
    struct timeval tv;

    // do not wait on select
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    maxfd = fdManager.setFDs(&readfds);
    select(maxfd+1, &readfds, NULL, NULL, &tv);
    fdManager.notify(&readfds);
}

#if 0
void MainWindow::displayfnDay(void)
{
    checkSockets();

    /* Clear the screen */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
    if (showMap)
    {
	MapProxy& mapProxy = MapProxy::instance();

	glLoadIdentity();

	glScalef((GLfloat)mapScale, (GLfloat)mapScale, 1.0);
	glRotatef((GLfloat)Rad2Deg(mapRotate), 0.0, 0.0, 1.0);
  	glTranslatef(-mapEye.x, -mapEye.y, -mapEye.z);

	mapProxy.draw(thumb);
	course->draw(orienteerProxy.getNextControlNumber());

	if (showLocation)
	    orienteerProxy.drawToMap(false);

	if (showRoute)
	    orienteerProxy.drawRunToMap();

	orienteerProxy.drawCompass(winWidth, winHeight);

	if (showRoutes)
	{
	    Database& db = Database::instance();
	    Database::Runs runs = db.getSelectedRuns2();
	    Run* selectedRun;
	    try
	    {
		selectedRun = db.getSelectedRun();
	    }
	    catch (const NotFound&)
	    {
		selectedRun = NULL;
	    }
	    for (Database::Runs::iterator ri = runs.begin(); ri != runs.end(); ++ri)
	    {
		Run* run = *ri;
		run->draw(run == selectedRun);
	    }
	}
    }
    else
    {
	orienteerProxy.adjustStepLength(standingStill);
	orienteerProxy.newPosition();
	orienteerProxy.adjustHorizontal(lookingAround);
	orienteerProxy.viewDay();
    }

    // Update fps
    sprintf(fpsTxt, "fps: %.02f", framesPerSecond);
    fps_text->setLabel(fpsTxt);

    windowMessages->display();

    /* Make PUI redraw */
    puDisplay();
  
    /* Off we go again... */
    glutSwapBuffers();
    glutPostRedisplay();
}
#endif

void MainWindow::displayMapDay(void)
{
    checkSockets();
    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
	MapProxy& mapProxy = MapProxy::instance();
	glLoadIdentity();
	glScalef((GLfloat)mapScale, (GLfloat)mapScale, 1.0);
	glRotatef((GLfloat)Rad2Deg(mapRotate), 0.0, 0.0, 1.0);
  	glTranslatef(-mapEye.x, -mapEye.y, -mapEye.z);
	mapProxy.draw(thumb);
	course->draw(orienteerProxy.getNextControlNumber());
	if (showLocation)
	    orienteerProxy.drawToMap(false);
	if (showRoute)
	    orienteerProxy.drawRunToMap();
	orienteerProxy.drawCompass(winWidth, winHeight);
	if (showRoutes)
	{
	    Database& db = Database::instance();
	    Database::Runs runs = db.getSelectedRuns2();
	    Run* selectedRun;
	    try
	    {
		selectedRun = db.getSelectedRun();
	    }
	    catch (const NotFound&)
	    {
		selectedRun = NULL;
	    }
	    for (Database::Runs::iterator ri = runs.begin(); ri != runs.end(); ++ri)
	    {
		Run* run = *ri;
		run->draw(run == selectedRun);
	    }
	}

    // Update fps
    sprintf(fpsTxt, "fps: %.02f", framesPerSecond);
    fps_text->setLabel(fpsTxt);

    windowMessages->display();

    /* Make PUI redraw */
    puDisplay();
  
    /* Off we go again... */
    glutSwapBuffers();
    glutPostRedisplay();
}

void MainWindow::displayTerrainDay(void)
{
    checkSockets();
    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
    orienteerProxy.adjustStepLength(standingStill);
    orienteerProxy.newPosition();
    orienteerProxy.adjustHorizontal(lookingAround);
    orienteerProxy.viewDay();
    // Update fps
    sprintf(fpsTxt, "fps: %.02f", framesPerSecond);
    fps_text->setLabel(fpsTxt);
    windowMessages->display();

    /* Make PUI redraw */
    puDisplay();
  
    /* Off we go again... */
    glutSwapBuffers();
    glutPostRedisplay();
}

void MainWindow::displayTerrainNight(void)
{
    checkSockets();
    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
    orienteerProxy.adjustStepLength(standingStill);
    orienteerProxy.newPosition();
    orienteerProxy.adjustHorizontal(lookingAround);
    orienteerProxy.viewNight();
    // Update fps
    sprintf(fpsTxt, "fps: %.02f", framesPerSecond);
    fps_text->setLabel(fpsTxt);
    windowMessages->display();
    /* Make PUI redraw */
    puDisplay();
  
    /* Off we go again... */
    glutSwapBuffers();
    glutPostRedisplay();
}

void MainWindow::callContinuation(void)
{
    pushDisplayFunc(MainWindow::terrainDF);
    if (continuationFunc != NULL)
	continuationFunc();
}

void MainWindow::generateMapDisplayFn(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MapProxy& mapProxy = MapProxy::instance();
    float percentComplete = mapProxy.continueRealize();

    if (percentComplete < 1.0)
    {
	dlgProgress->progress(percentComplete);
    }
    else
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlgBase = dlgStack.pop();
	puDeleteObject(dlgBase);

	EventProxy& eventProxy = EventProxy::instance();
	eventProxy.realize();
	setUniform1f(dayShader, "TerrainLow", eventProxy.getLowestPoint());
	setUniform1f(dayShader, "TerrainHigh", eventProxy.getHighestPoint());
	setUniform1f(nightShader, "TerrainLow", eventProxy.getLowestPoint());
	setUniform1f(nightShader, "TerrainHigh", eventProxy.getHighestPoint());

	OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
	int id = (client != NULL) ? client->getId() : 1;
	orienteerProxy.reset(id);
	orienteerProxy.setEventProxy(&eventProxy);
	
	Database& db = Database::instance();
	course = db.getEventCourse();
	callContinuation();
    }

    puDisplay();
  
    glutSwapBuffers();
    glutPostRedisplay();
}

void MainWindow::generateTerrainDisplayFn(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TerrainProxy& terrainProxy = TerrainProxy::instance();
    float percentComplete = terrainProxy.continueRealize();

    if (percentComplete < 1.0)
    {
	dlgProgress->progress(percentComplete);
    }
    else
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlgBase = dlgStack.pop();
	puDeleteObject(dlgBase);

	terrainProxy.finishRealize();
	realizeMap();
    }
    
    puDisplay();
  
    glutSwapBuffers();
    glutPostRedisplay();
}

void MainWindow::realizeMap()
{
    MapProxy& mapProxy = MapProxy::instance();
    float percentComplete = mapProxy.initRealize();

    if (percentComplete < 1.0)
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlg = dlgStack.top();
	dlgProgress = new DlgProgress(dlg, "Drawing map");
	dlgStack.push(dlgProgress);
	pushDisplayFunc(MainWindow::generateMapDisplayFn);
    }
    else
    {
	callContinuation();
    }
}

void MainWindow::pushDisplayFuncs(void)
{
    Database& db = Database::instance();
    Event* event = db.getSelectedEvent();
    if (event->night()) 
    {
        MainWindow::terrainDF = MainWindow::displayTerrainNight;
	MainWindow::mapDF = MainWindow::displayMapDay;
    }
    else
    {
        MainWindow::terrainDF = MainWindow::displayTerrainDay;
	MainWindow::mapDF = MainWindow::displayMapDay;
    }
    MainWindow::pushDisplayFunc(MainWindow::terrainDF);
}

void MainWindow::realizeTerrain(continuationFunction cf)
{
    continuationFunc = cf;

    pushDisplayFuncs();

    TerrainProxy& terrainProxy = TerrainProxy::instance();
    float percentComplete = terrainProxy.initRealize();

    if (percentComplete < 1.0)
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlg = dlgStack.top();
	dlgProgress = new DlgProgress(dlg, "Generating terrain");
	dlgStack.push(dlgProgress);
	pushDisplayFunc(MainWindow::generateTerrainDisplayFn);
    }
    else
    {
	realizeMap();
    }
}

void MainWindow::reshapefn(int width, int height)
{
    MainWindow::winWidth = width;
    MainWindow::winHeight = height;
    MainWindow::projection();
    DlgStack::instance().reshape(50, 50, width-100, height-100);
    MainWindow::windowMessages->reshape(winWidth, winHeight);
}

void MainWindow::single_start_continuation()
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlg = dlgStack.top();
    DlgConfirm::confirm(dlg, "Quit this run?", 
			"Quit", quit_run_ok_cb, 
			"Cancel", quit_run_cancel_cb);
    mainWindow.startCountdown();
}

void MainWindow::view_results_continuation()
{
    showMap = true;
    showRoutes = true;
    MainWindow::displaySwitch();
}

void MainWindow::save_run()
{
    Database& db = Database::instance();
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    Run* run = oriProxy.getRun();
    try
    {
        db.addRun(run);
        db.updateRunsView();
        db.setSelectedRun(run);
        db.updateRunObservers(0);
    }
    catch (const Exists&)
    {
        std::cout << "Run exists in the database\n";
    }
}

void MainWindow::startRace()
{
    OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
    orienteerProxy.start();
    bool game_on, show_dialogs;
    MainWindow::resetState(game_on = true, show_dialogs = false);
}

void MainWindow::stopRace()
{
    windowMessages->cancelTimer();
    windowMessages->setMessage(WindowMessages::HA_CENTER,
			       WindowMessages::VA_CENTER,
			       "");

    if (gameOn)
    {
        OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
        orienteerProxy.quit();
    }

    bool game_on, show_dialogs;
    MainWindow::resetState(game_on = false, show_dialogs = true);
}

void MainWindow::timeout(int userData)
{
    if (userData == 0)
    {
        windowMessages->setMessage(WindowMessages::HA_CENTER, 
                                   WindowMessages::VA_CENTER,
                                   "");
    }
    else if (userData == 1)
    {
        windowMessages->setMessage(WindowMessages::HA_CENTER, 
                                   WindowMessages::VA_CENTER,
                                   "Go!");
        windowMessages->startTimer(1000, this, userData-1);
        startRace();
    }
    else
    {
        std::ostringstream ostr;
        ostr << "Start in " << userData-1; 
        windowMessages->setMessage(WindowMessages::HA_CENTER, 
                                   WindowMessages::VA_CENTER,
                                   ostr.str());
        windowMessages->startTimer(1000, this, userData-1);
    }
}

void MainWindow::startCountdown()
{
    OrienteerProxy& orienteerProxy = OrienteerProxy::instance();
    int id = (client != NULL) ? client->getId() : 1;
    orienteerProxy.reset(id);

    DlgStack::instance().setVisible(false);
    windowMessages->setMessage(WindowMessages::HA_CENTER, 
                               WindowMessages::VA_CENTER,
                               "Start in 5");
    windowMessages->startTimer(1000, this, 5);
    bool game_on, show_dialogs;
    MainWindow::resetState(game_on = false, show_dialogs = true);
}

void MainWindow::serverExited()
{
    delete server;
    server = NULL;
#if 0
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
#endif
}

void MainWindow::startServer(unsigned short portNumber)
{
    if (server != NULL)
    {
        delete server;
    }
    server = new Server(portNumber, fdManager, &mainWindow);
}


#ifdef WIN32
void MainWindow::socketStartup()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
 
    wVersionRequested = MAKEWORD(2, 2);
 
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) 
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return;
    }
 
    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */
    
    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        WSACleanup();
        return; 
    }
}

void MainWindow::socketCleanup()
{
    WSACleanup();
}
#endif


Server* MainWindow::getServer()
{
    return server;
}

Client* MainWindow::getClient()
{
    return client;
}

void MainWindow::connect(std::string hostName, unsigned short portNumber)
{
    if (client != NULL)
    {
        delete client;
    }

    Database& db = Database::instance();
    client = new Client(3490, fdManager, &mainWindow);
    client->connect(hostName, portNumber);

    Orienteer* orienteer = db.getSelectedOrienteer();
    client->sendJoinReq(orienteer->name());
}


void MainWindow::show_location()
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    oriProxy.disqualify();
    if (client != 0)
        client->sendCheat();
    showLocation = !showLocation;
}


void MainWindow::initLights()
{
    static GLfloat day_clear_r,day_clear_g,day_clear_b;
    static GLfloat day_fog_r,day_fog_g, day_fog_b;
    static GLfloat night_clear_r, night_clear_g, night_clear_b;
    static GLfloat night_fog_r, night_fog_g, night_fog_b;

    static GLfloat day_fog_color[4];
    static GLfloat night_fog_color[4];

    static float lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};

    static float diffuse[] = {1.0, 1.0, 1.0, 1.0};
    static float specular[] = {0.0, 0.0, 0.0, 1.0};
    static float ambient[] = {0.3, 0.3, 0.3, 1.0};

    static float front_mat_shininess[] = {10.0};
    static float front_mat_specular[] = {0.0, 0.0, 0.0, 1.0};
    static float front_mat_diffuse[] = {0.1, 0.7, 0.3, 1.0};

    //glColor3f(0.541, 0.6196, 1.0); // light blue 

    day_clear_r   = 0.541; day_clear_g   = 0.6196; day_clear_b = 1.0;

    //    day_clear_r   = 0.0; day_clear_g   = 0.746; day_clear_b = 1.0;
    day_fog_r     = 1.0; day_fog_g     = 1.0; day_fog_b     = 1.0;
    night_clear_r = 0.0; night_clear_g = 0.0; night_clear_b = 0.05;
    night_fog_r   = 0.0; night_fog_g   = 0.0; night_fog_b   = 0.05;

    day_fog_color[0] = day_fog_r;
    day_fog_color[1] = day_fog_g;
    day_fog_color[2] = day_fog_b;
    day_fog_color[3] = 1.0;

    night_fog_color[0] = night_fog_r;
    night_fog_color[1] = night_fog_g;
    night_fog_color[2] = night_fog_b;
    night_fog_color[3] = 1.0;

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_mat_diffuse);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    glClearColor(day_clear_r, day_clear_g, day_clear_b, 1.0);
    glDisable(GL_FOG);

}

void MainWindow::init()
{
    // init gui
    fps_text = new puText(10, 10);
    fps_text->setColour(PUCOL_LABEL, 1.0, 1.0, 1.0);

    windowMessages = new WindowMessages(winWidth, winHeight);

    DlgMenu* dlgMenu = new DlgMenu(0, 50, 50);
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.push(dlgMenu);
    dlgStack.reshape(50, 50, winWidth-100, winHeight-100);

    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    oriProxy.setOpObserver(&mainWindow);

    // init database
    Database& db = Database::instance();
    const XmlParser& xmlParser = XmlParser::instance();
    xmlParser.readFile("batbdb.xml", db);
    db.ensureMinimalDB();
    db.addObserver(&mainWindow);

    // init glut display functions
    MainWindow::pushDisplayFuncs();

    initTextures();
    MainWindow::displayTerrain();
    MainWindow::projection();

    bool game_on, show_dialogs;
    MainWindow::resetState(game_on = false, show_dialogs = true);

    MainWindow::realizeTerrain(NULL);

#ifdef WIN32
    socketStartup();
#endif
}


void MainWindow::resetState(bool game_on, bool show_dialogs)
{
    mouseButton = 0;
    showMap = false;
    standingStill = true;
    gameOn = game_on;
    typingMessage = false;
    showDialogs = show_dialogs;
    showFps = false;
    if (showFps)
        fps_text->reveal();
    else
        fps_text->hide();
    fullScreen = false;
    showLocation = false;
    showRoute = false;
    showRoutes = false;
    settingLocation = false;

    mapScale = 1.0;
    mapScaleMin = 0.2;
    mapScaleMax = 10.0;
    mapEye.x = 0;
    mapEye.y = 0;

    thumb.x = thumb.y = 0;

    displaySwitch();
}

int MainWindow::updateEvents(void* callData) const
{
    if (server != NULL && server->getExiting() == false)
    {
        server->sendEventInfoAll();
    }
    showRoutes = false;
    return 0;
}

void MainWindow::chat(std::string message)
{
    windowMessages->addMessage(message);
}

void MainWindow::punch(int controlNumber)
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    bool finished = oriProxy.punch();
    if (finished)
    {
        windowMessages->setMessage(WindowMessages::HA_CENTER, 
                                   WindowMessages::VA_CENTER,
                                   "You have finished!");
        windowMessages->startTimer(5000, this, 0);

        DlgStack& dlgStack = DlgStack::instance();
        dlgStack.pop();

	bool game_on, show_dialogs;
	MainWindow::resetState(game_on = false, show_dialogs = false);

        oriProxy.finish();
        if (client != NULL)
        {
            client->sendFinished(oriProxy.getTotalTime());
        }
        else
        {
            save_run();
        }
    }
    else
    {
        if (client != NULL)
        {
            client->sendPunch(controlNumber);
        }
    }
}

static void printVersions()
{
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    //const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    const GLubyte* GLUversion = gluGetString(GLU_VERSION);

    std::cout << "Vendor: " << vendor << "\n";
    std::cout << "Renderer: " << renderer << "\n";
    std::cout << "OpenGL Version: " << version << "\n";
#ifdef GL_SHADING_LANGUAGE_VERSION
    const GLubyte* SLversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (SLversion != 0) {
	std::cout << "OpenGL Shading Language Version: " << SLversion << "\n";
    }
#endif
#ifdef GL_MAX_TEXTURE_UNITS
    GLint maxTextureUnits[1];
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, maxTextureUnits);
    std::cout << "Max texture units: " << maxTextureUnits[0] << "\n";
#endif
    //std::cout << "Extensions: " << extensions << "\n";
    std::cout << "GLU Version: " << GLUversion << std::endl;
}

std::vector<MainWindow::DisplayFunc> MainWindow::displayFuncs;
bool MainWindow::displayFuncEnabled = true;

void MainWindow::updateDisplayFunc()
{
    try 
    {
        // disable display function only if it is the normal display func
	// do not disable the terrain or map generation funcs 
        // (because the display function drives the terrain and map generation
        // so if it is disabled, nothing would be generated)
	DisplayFunc displayFunc = displayFuncs.at(displayFuncs.size()-1);
	if (!displayFuncEnabled && displayFunc == MainWindow::terrainDF)
	{
	    //glutDisplayFunc(NULL);
	    glutIdleFunc(NULL);
	}
	else
	{
	    glutDisplayFunc(displayFunc);
	    glutIdleFunc(displayFunc);
	}
    }
    catch (std::out_of_range& ex)
    {
    }
}

void MainWindow::setDisplayFunc(DisplayFunc displayFunc)
{
    if (displayFuncs.size() > 0) {
        // remove last
        displayFuncs.pop_back();
    }
    displayFuncs.push_back(displayFunc);
    updateDisplayFunc();
}

void MainWindow::pushDisplayFunc(DisplayFunc displayFunc)
{
    displayFuncs.push_back(displayFunc);
    updateDisplayFunc();
}

void MainWindow::popDisplayFunc()
{
    displayFuncs.pop_back();
    updateDisplayFunc();
}

void MainWindow::enableDisplayFunc()
{
    displayFuncEnabled = true;
    updateDisplayFunc();
}

void MainWindow::disableDisplayFunc()
{
    displayFuncEnabled = false;
    updateDisplayFunc();
}

void MainWindow::visibility(int state)
{ 
    switch (state)
    {

    case GLUT_NOT_VISIBLE:
	disableDisplayFunc();
	break;
    case GLUT_VISIBLE:
	enableDisplayFunc();
	break;
    }
}

/***************************************************************************/
/* Parse GL_VERSION and return the major and minor numbers in the supplied
 * integers.
 * If it fails for any reason, major and minor will be set to 0.
 * Assumes a valid OpenGL context.
*/

static void getGlVersion( int *major, int *minor )
{
    const char* verstr = (const char*)glGetString( GL_VERSION );
    if( (verstr == NULL) || (sscanf( verstr, "%d.%d", major, minor ) != 2) )
    {
        *major = *minor = 0;
        fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
    }
}

int main(int argc, char **argv)
{
    MainWindow::winPosX = 100;
    MainWindow::winPosY = 100;
    MainWindow::winWidth = 640;
    MainWindow::winHeight = 480;

    // init glut
    glutInitWindowPosition(MainWindow::winPosX, MainWindow::winPosY);
    glutInitWindowSize    (MainWindow::winWidth, MainWindow::winHeight);
    glutInit              (&argc, argv);
    glutInitDisplayMode   (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    MainWindow::main_window = glutCreateWindow("Beat About the Bush");
    MainWindow::enableDisplayFunc();

    glutKeyboardFunc      (MainWindow::keyDownFn);
    //glutKeyboardUpFunc    (MainWindow::keyUpFn);
    glutSpecialFunc       (MainWindow::specialDownFn);
    //glutSpecialUpFunc     (MainWindow::specialUpFn);
    glutMouseFunc         (MainWindow::mousefn);
    glutMotionFunc        (MainWindow::motionfn);
    glutReshapeFunc       (MainWindow::reshapefn);
    glutVisibilityFunc    (MainWindow::visibility);

    // init plib
    puInit();

    puSetDefaultStyle        ( PUSTYLE_SMALL_SHADED );
    puSetDefaultColourScheme ( 0.3f, 0.4f, 0.6f, 1.0f);

    // Initialize the "OpenGL Extension Wrangler" library
    glewInit();

    mainWindow.initLights();
    mainWindow.init();
    printVersions();

    // Make sure that OpenGL 2.0 is supported by the driver
    int gl_major, gl_minor;
    getGlVersion(&gl_major, &gl_minor);
    printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);

    if (gl_major < 2)
    {
        printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
        printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
        exit(1);
    }

    // init shaders
    GLchar *dayVSSource, *dayFSSource;
    readShaderSource("shader/day", &dayVSSource, &dayFSSource);
    dayShader = installShaders(dayVSSource, dayFSSource);
    float forestGreen[] = {34.0/255, 139.0/255, 34.0/255};
    //float auburn[] = {113.0/255, 47.0/255, 38.0/255};
    float grey[] = {0.5, 0.5, 0.5};
    //float sepia[] = {112.0/255, 66.0/255, 20.0/255};

    setUniform3f(dayShader, "GrassColor", forestGreen[0], forestGreen[1], forestGreen[2]);
    setUniform3f(dayShader, "RockColor", grey[0], grey[1], grey[2]);
    //setUniform3f(dayShader, "DirtColor", sepia[0], sepia[1], sepia[2]);
    //setUniform4f(dayShader, "LightPos", 0.0, 0.0, 100.0, 1.0);
    setUniform1f(dayShader, "Scale", 1.0);
    setUniform1f(dayShader, "TerrainHigh", 0.1);
    setUniform1f(dayShader, "TerrainLow", 0.1);
    setUniform1i(dayShader, "Trees", 0); // sampler
    setUniform1i(dayShader, "AltGrad", 1); // sampler
    setUniform1i(dayShader, "Noise", 2); // sampler

    GLchar *nightVSSource, *nightFSSource;
    readShaderSource("shader/night3", &nightVSSource, &nightFSSource);
    nightShader = installShaders(nightVSSource, nightFSSource);
    setUniform3f(nightShader, "BrickColor", 1.0, 0.3, 0.2);
    setUniform3f(nightShader, "MortarColor", 0.85, 0.86, 0.84);
    setUniform2f(nightShader, "BrickSize", 0.30, 0.15);
    setUniform2f(nightShader, "BrickPct", 0.90, 0.85);
    setUniform1i(nightShader, "numEnabledLights", 2);

    setUniform3f(nightShader, "GrassColor", forestGreen[0], forestGreen[1], forestGreen[2]);
    setUniform3f(nightShader, "RockColor", grey[0], grey[1], grey[2]);
    setUniform1f(nightShader, "Scale", 1.0);
    setUniform1f(nightShader, "TerrainHigh", 0.1);
    setUniform1f(nightShader, "TerrainLow", 0.1);
    setUniform1i(nightShader, "Trees", 0); // sampler
    setUniform1i(nightShader, "AltGrad", 1); // sampler
    setUniform1i(nightShader, "Noise", 2); // sampler

    GLchar *nightTreeVS, *nightTreeFS;
    readShaderSource("shader/nightTree", &nightTreeVS, &nightTreeFS);
    nightTrees = installShaders(nightTreeVS, nightTreeFS);
    setUniform1i(nightTrees, "Trees", 0); // sampler

    // enter main loop
    if (dayShader && nightShader)
        glutMainLoop();

    return 0;
}

void MainWindow::single_start_cb(puObject *)
{
    realizeTerrain(MainWindow::single_start_continuation);
}

void MainWindow::view_results_cb(puObject *)
{
    realizeTerrain(MainWindow::view_results_continuation);
}

void MainWindow::kick_ok_cb(puObject*)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.pop();
    DlgHost2* dlgHost2 = (DlgHost2*)dlgStack.top();
    int si = dlgHost2->getSelectedIndex();
    server->kickClient(si);
}

void MainWindow::save_run_ok_cb(puObject *dlg)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlgConfirm = dlgStack.pop();
    puDeleteObject(dlgConfirm);
    save_run();
}

void MainWindow::save_run_cancel_cb(puObject *dlg)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlgConfirm = dlgStack.pop();
    puDeleteObject(dlgConfirm);
}

void MainWindow::quit_run_ok_cb(puObject *)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);

    dlg = dlgStack.top();
    DlgConfirm::confirm(dlg, "Save this run?", 
                        "Save", save_run_ok_cb, 
                        "Cancel", save_run_cancel_cb);
    dlgStack.setVisible(showDialogs = true);
    stopRace();
}

void MainWindow::quit_run_cancel_cb(puObject *)
{
    showDialogs = false;
    DlgStack::instance().setVisible(showDialogs);
}

void MainWindow::host2_start_race_cb(puObject *dlg)
{
    DlgBase* dlgBase = (DlgBase*)dlg;
    dlgBase->setRaceOn(true);
    server->sendStartRaceAll();
}

void MainWindow::host2_stop_race_cb(puObject *dlg)
{
    DlgBase* dlgBase = (DlgBase*)dlg;
    dlgBase->setRaceOn(false);
    server->sendStopRaceAll();
}

void MainWindow::host2_stop_server_cb(puObject *dlg)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgHost2* dlgHost2 = (DlgHost2*)dlgStack.pop();
    server->removeClientObserver(dlgHost2);
    puDeleteObject(dlgHost2);
    
    server->setExiting(true);
    if (server->getClientCount() == 0)
    {
        mainWindow.serverExited();
    }
    else
    {
        server->kickAllClients();
    }
}

void MainWindow::host1_start_server_cb(puObject *dlg)
{
    DlgHost1* dlgHost1 = (DlgHost1*)dlg->getUserData();
    std::string port = dlgHost1->getPortNumber();

    if (port == "")
    {
        DlgInformation::info(dlgHost1, "No port number given");
    }
    else
    {
        unsigned short portNumber = atoi(port.c_str());
        try
        {
            startServer(portNumber);

            DlgHost2* dlgHost2 = 
                new DlgHost2(dlgHost1, 50, 50,
                            &MainWindow::host2_start_race_cb,
                            &MainWindow::host2_stop_race_cb, 
                            &MainWindow::host2_stop_server_cb);
            server->addClientObserver(dlgHost2);
            DlgStack::instance().push(dlgHost2);

            // if not dedicated server
            connect("localhost", portNumber);
        } 
        catch (TcpListenerError& )
        {
            DlgInformation::info(dlgHost1, "This port is in use, choose another port number");
        }
    }
}

void MainWindow::join1_connect_cb(puObject *dlg)
{
    DlgJoin1* dlgJoin1 = (DlgJoin1*)dlg->getUserData();
    std::string hostName = dlgJoin1->getHostName();
    std::string port = dlgJoin1->getPortNumber();

    if (hostName == "")
    {
        DlgInformation::info(dlgJoin1, "No hostname given");
    }
    else if (port == "")
    {
        DlgInformation::info(dlgJoin1, "No port number given");
    }
    else
    {
        Database& db = Database::instance();
        db.addServer(hostName);

        unsigned short portNumber = atoi(port.c_str());
        try
        {
            connect(hostName, portNumber);
        } 
        catch (ConnectError&)
        {
            DlgInformation::info(dlgJoin1, "Cannot connect");
        }
    }
}

void MainWindow::show_location_ok_cb(puObject *cb)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.setVisible(showDialogs = false);
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);
    show_location();
}

void MainWindow::show_location_cancel_cb(puObject *cb)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.setVisible(showDialogs = false);
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);
}

void MainWindow::show_location_cb(puObject *cb)
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    if (oriProxy.getStatus() == Run::OK)
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlg = dlgStack.top();
	DlgConfirm::confirm(dlg, 
			    "Your run will be disqualified.\n"
			    "        Are you sure?", 
			    "Show", show_location_ok_cb, 
			    "Cancel", show_location_cancel_cb);
	dlgStack.setVisible(showDialogs = true);
    }
    else
    {
	show_location();
    }
}

void MainWindow::show_route()
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    oriProxy.disqualify();
    if (client != 0)
        client->sendCheat();
    showRoute = !showRoute;
}

void MainWindow::show_route_ok_cb(puObject *cb)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.setVisible(showDialogs = false);
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);
    show_route();
}

void MainWindow::show_route_cancel_cb(puObject *cb)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.setVisible(showDialogs = false);
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);
}

void MainWindow::show_route_cb(puObject *cb)
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    if (oriProxy.getStatus() == Run::OK)
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlg = dlgStack.top();
	DlgConfirm::confirm(dlg, 
			    "Your run will be disqualified.\n"
			    "        Are you sure?", 
			    "Show", show_route_ok_cb, 
			    "Cancel", show_route_cancel_cb);
	dlgStack.setVisible(showDialogs = true);
    }
    else
    {
	show_route();
    }
}

void MainWindow::set_location()
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    oriProxy.disqualify();
    if (client != 0)
        client->sendCheat();
    settingLocation = true;
    windowMessages->setMessage(WindowMessages::HA_CENTER,
                               WindowMessages::VA_BOTTOM,
                               "Set position by clicking on map");
}

void MainWindow::set_location_ok_cb(puObject *cb)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.setVisible(showDialogs = false);
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);
    set_location();
}

void MainWindow::set_location_cancel_cb(puObject *cb)
{
    DlgStack& dlgStack = DlgStack::instance();
    dlgStack.setVisible(showDialogs = false);
    DlgBase* dlg = dlgStack.pop();
    puDeleteObject(dlg);
    settingLocation = false;
}

void MainWindow::set_location_cb(puObject *cb)
{
    OrienteerProxy& oriProxy = OrienteerProxy::instance();
    if (oriProxy.getStatus() == Run::OK)
    {
	DlgStack& dlgStack = DlgStack::instance();
        DlgBase* dlg = dlgStack.top();
        DlgConfirm::confirm(dlg, 
                            "Your run will be disqualified.\n"
                            "        Are you sure?", 
                            "Show", set_location_ok_cb, 
                            "Cancel", set_location_cancel_cb);
	dlgStack.setVisible(showDialogs = true);
    }
    else
    {
        set_location();
    }
}

void MainWindow::quit_ok_cb(puObject *)
{
    Database& db = Database::instance();
    db.write("batbdb.xml");
    db.discard();

    delete windowMessages;

    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlgConfirm = dlgStack.pop();
    delete dlgConfirm;
    DlgBase* dlgMenu = dlgStack.pop();
    delete dlgMenu;
    puDeleteObject(fps_text);
#ifdef WIN32
    socketCleanup();
#endif

    exit(0);
}

void MainWindow::join2_quit_cb(puObject *dlg)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgJoin2* dlgJoin2 = (DlgJoin2*)dlgStack.top();
    dlgJoin2->setRaceOn(false);
    client->sendQuit();
    stopRace();
}

void MainWindow::join2_disconnect_cb(puObject *dlg)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgJoin2* dlgJoin2 = (DlgJoin2*)dlgStack.pop();
    client->removeClientObserver(dlgJoin2);
    puDeleteObject(dlgJoin2);

    client->sendDiscReq();
}


// Begin MessageHandler implementation

bool MainWindow::handle(const MsgJoinAccept& msg, TcpServer* fromTcpServer)
{
    int id = msg.getId();
    client->setId(id);

    if (server == NULL)
    {
	DlgStack& dlgStack = DlgStack::instance();
	DlgBase* dlgBase = dlgStack.top();
        DlgJoin2* dlgJoin2 = new DlgJoin2(dlgBase, 50, 50, join2_quit_cb, join2_disconnect_cb);
        client->addClientObserver(dlgJoin2);
        dlgStack.push(dlgJoin2);
    }

    return true;
}

bool MainWindow::handle(const MsgJoinReject& msg, TcpServer* fromTcpServer)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlg = dlgStack.top();
    DlgInformation::info(dlg, "Joining game rejected by server");
    return true;
}

void MainWindow::msgEventInfoContinuation()
{
    client->sendReadyToGo();
}

bool MainWindow::handle(const MsgEventInfo& msg, TcpServer* fromTcpServer)
{
    Database& db = Database::instance();
    Terrain* terrain = msg.getTerrain();
    Map* map = msg.getMap();
    Course* course = msg.getCourse();
    Event* event = msg.getEvent();
    db.addMPEvent(terrain, map, course, event);

    //generate everything so that we are ready to go
    realizeTerrain(MainWindow::msgEventInfoContinuation);

    return true;
}

bool MainWindow::handle(const MsgStartRace& msg, TcpServer* fromTcpServer)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgBase* dlg = dlgStack.top();
    dlg->setRaceOn(true);

    mainWindow.startCountdown();
    return true;
}

bool MainWindow::handle(const MsgStopRace& msg, TcpServer* fromTcpServer)
{
    client->sendQuit();
    stopRace();
    return true;
}

bool MainWindow::handle(const MsgNewClient& msg, TcpServer* fromTcpServer)
{
    int id = msg.getId();
    std::string ip = msg.getIp();
    int port = msg.getPort();
    std::string name = msg.getName();
    UdpClient* udpClient = new UdpClient(id, ip, port, name);

    client->addUdpClient(udpClient);
    client->printUdpClients();
    client->updateClientsView();
    client->notifyClientObservers();

    EventProxy& eventProxy = EventProxy::instance();
    eventProxy.addObserver(udpClient);

    return true;
}

bool MainWindow::handle(const MsgRemoveClient& msg, TcpServer* fromTcpServer)
{
    int id = msg.getId();
    UdpClient* udpClient = client->removeUdpClient(id);
    EventProxy& eventProxy = EventProxy::instance();
    eventProxy.removeObserver(udpClient);
    delete udpClient;

    client->printUdpClients();
    client->updateClientsView();
    client->notifyClientObservers();

    return true;
}

bool MainWindow::handle(const MsgChat& msgChat, UdpServer* fromUdpServer)
{
    int id = msgChat.getId();
    UdpClient* udpClient = client->udpClientById(id);

    if (udpClient != 0) 
    {
        std::string message = udpClient->getName();
        message += ": " + msgChat.getMessage();
        windowMessages->addMessage(message);
    }
    else 
    {
        std::cout << "handleMsgChat: client " << id << " not found\n";
    }
    return true;
}

bool MainWindow::handle(const MsgPosition& msgPosition, UdpServer* fromUdpServer)
{
    int id = msgPosition.getId();
    UdpClient* udpClient = client->udpClientById(id);

    if (udpClient != 0) 
    {
        udpClient->setLocation(msgPosition.getX(), msgPosition.getY(),
                               msgPosition.getZ(),
                               msgPosition.getHorizontal(),
                               msgPosition.getVertical());
    }
    else 
    {
        std::cout << "handleMsgPosition: client " << id << " not found\n";
    }
    return true;
}

bool MainWindow::handle(const MsgPunch& msg, UdpServer* fromUdpServer)
{
    int id = msg.getId();
    UdpClient* udpClient = client->udpClientById(id);
    if (udpClient != 0) 
    {
	int controlNumber = msg.getControlNumber();
	udpClient->punch(controlNumber);
	client->updateClientsView();
	client->notifyClientObservers();
    }
    else 
    {
        std::cout << "handleMsgPunch: client " << id << " not found\n";
    }
    return true;
}

bool MainWindow::handle(const MsgFinished& msg, UdpServer* fromUdpServer)
{
    int id = msg.getId();
    UdpClient* udpClient = client->udpClientById(id);
    if (udpClient != 0) 
    {
	udpClient->setState(UdpClient::finished);
	client->updateClientsView();
	client->notifyClientObservers();
    }
    else 
    {
        std::cout << "handleMsgFinished: client " << id << " not found\n";
    }
    return true;
}

bool MainWindow::handle(const MsgQuit& msg, UdpServer* fromUdpServer)
{
    int id = msg.getId();
    UdpClient* udpClient = client->udpClientById(id);
    if (udpClient != 0) 
    {
	udpClient->setState(UdpClient::quit);
	client->updateClientsView();
	client->notifyClientObservers();
    }
    else 
    {
        std::cout << "handleMsgQuit: client " << id << " not found\n";
    }
    return true;
}

bool MainWindow::handle(const MsgCheat& msg, UdpServer* fromUdpServer)
{
    int id = msg.getId();
    UdpClient* udpClient = client->udpClientById(id);
    if (udpClient != 0) 
    {
	udpClient->setState(UdpClient::cheated);
	client->updateClientsView();
	client->notifyClientObservers();
    }
    else 
    {
        std::cout << "handleMsgCheat: client " << id << " not found\n";
    }
    return true;
}

bool MainWindow::handle(const MsgJoinReq& msgJoinReq, TcpClient* tcpClient)
{
    if (server->getExiting() == true)
    {
        server->removeTcpClient(tcpClient);
        tcpClient->sendJoinReject();
    }
    else
    {
        tcpClient->sendJoinAccept();
        tcpClient->setName(msgJoinReq.getName());
        tcpClient->setPort(msgJoinReq.getPort());
        server->sendNewClientAll(tcpClient);
        tcpClient->sendEventInfo();
        server->printTcpClients();
        server->updateClientsView();
        server->notifyClientObservers();
    }
    return true;
}

bool MainWindow::handle(const MsgReadyToGo& msg, TcpClient* sender)
{
    sender->setState(TcpClient::ready);
    server->updateClientsView();
    server->notifyClientObservers();
    return true;
}

bool MainWindow::handle(const MsgPunch& msg, TcpClient* sender)
{
    int controlNumber = msg.getControlNumber();
    sender->punch(controlNumber);
    server->updateClientsView();
    server->notifyClientObservers();
    return true;
}

bool MainWindow::handle(const MsgFinished& msg, TcpClient* sender)
{
    sender->setState(TcpClient::finished);
    server->updateClientsView();
    server->notifyClientObservers();
    return true;
}

bool MainWindow::handle(const MsgQuit& msg, TcpClient* sender)
{
    sender->setState(TcpClient::quit);
    server->updateClientsView();
    server->notifyClientObservers();
    return true;
}

bool MainWindow::handle(const MsgCheat& msg, TcpClient* sender)
{
    sender->setState(TcpClient::cheated);
    server->updateClientsView();
    server->notifyClientObservers();
    return true;
}

bool MainWindow::handle(const MsgKick& msg, TcpServer* fromTcpServer)
{
    DlgStack& dlgStack = DlgStack::instance();
    DlgJoin2* dlgJoin2 = (DlgJoin2*)dlgStack.pop();
    client->removeClientObserver(dlgJoin2);
    puDeleteObject(dlgJoin2);

    client->sendDiscReq();
    return true;
}

bool MainWindow::handle(const MsgDiscReq& msgDiscReq, TcpClient* tcpClient)
{
    server->removeTcpClient(tcpClient);
    server->sendRemoveClientAll(tcpClient);
    tcpClient->sendDiscConf();
    tcpClient->disconnect();
    delete tcpClient;

    server->printTcpClients();

    server->updateClientsView();
    server->notifyClientObservers();

    if ((server->getExiting() == true) && (server->getClientCount() == 0))
    {
        serverExited();
    }

    // we return false because we have deleted the client
    return false;
}

bool MainWindow::handle(const MsgDiscConf& msg, TcpServer* fromTcpServer)
{
    client->disconnectTcp();
    DlgStack& dlgStack = DlgStack::instance();
#if 0
    DlgBase* dlg = dlgStack.pop();
    dlg = dlgStack.top();
#else
    DlgBase* dlg = dlgStack.top();
#endif
    DlgInformation::info(dlg, "You are now disconnected");

    // we return false because we have deleted the client
    return false;
}

// End MessageHandler implementation

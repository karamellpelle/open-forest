#include <algorithm>

#include "database.h"
#include "matrix.h"
#include "conditions.h"
#include "terrainproxy.h"
#include "mapproxy.h"
#include "observer.h"
#include "eventproxy.h"
#include "oriproxy.h"
#include "mainWindow.h"

#include "dlgCommon.h"
#include "dlgOrienteer.h"
#include "dlgSelect.h"
#include "dlgEvent.h"
#include "dlgResult.h"
#include "dlgSingle.h"
#include "dlgOptions.h"
#include "dlgStack.h"
#include "dlgHost1.h"
#include "dlgHost2.h"
#include "dlgJoin1.h"
#include "dlgJoin2.h"



void DlgOptions::reshape(int x, int y, int width, int height)
{
    const int padding = 10;
    const int buttonWidth = 200;
    const int buttonHeight = 20;

    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    window_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -2*(buttonHeight + padding)));
    fullscreen_640x480_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 2*(buttonHeight + padding)));
    fullscreen_800x600_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 1*(buttonHeight + padding)));
    fullscreen_1024x800_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 0*(buttonHeight + padding)));
    fullscreen_1280x1024_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -1*(buttonHeight + padding)));
    ok_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -3*(buttonHeight + padding)));
}

void DlgOptions::window_cb(puObject *btn)
{
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
		glutLeaveGameMode();
}

void DlgOptions::fullscreen_640x480_cb(puObject *btn)
{
	glutGameModeString("640x480");
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
		glutEnterGameMode();
	else {
		printf("The select mode is not available\n");
	}
}

void DlgOptions::fullscreen_800x600_cb(puObject *btn)
{
	glutGameModeString("800x600");
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
		glutEnterGameMode();
	else {
		printf("The select mode is not available\n");
	}
}

void DlgOptions::fullscreen_1024x800_cb(puObject *btn)
{
	glutGameModeString("1024x800");
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
		glutEnterGameMode();
	else {
		printf("The select mode is not available\n");
	}
}

void DlgOptions::fullscreen_1280x1024_cb(puObject *btn)
{
	glutGameModeString("1280x1024");
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
		glutEnterGameMode();
	else {
		printf("The select mode is not available\n");
	}
}

void DlgOptions::ok_cb(puObject *btn)
{
}

void DlgOptions::init(puCallback ok_cb)
{
    int width = 600;
    int height = 400;
    const int buttonWidth = 200;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Options");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    window_button = new puOneShot(10, 10, "Window");
    window_button->setSize(buttonWidth, buttonHeight);
    window_button->setUserData(this);
    window_button->setCallback(window_cb);

    fullscreen_640x480_button = new puOneShot(10, 10, "Fullscreen 640x480");
    fullscreen_640x480_button->setSize(buttonWidth, buttonHeight);
    fullscreen_640x480_button->setUserData(this);
    fullscreen_640x480_button->setCallback(fullscreen_640x480_cb);

    fullscreen_800x600_button = new puOneShot(10, 10, "Fullscreen 800x600");
    fullscreen_800x600_button->setSize(buttonWidth, buttonHeight);
    fullscreen_800x600_button->setUserData(this);
    fullscreen_800x600_button->setCallback(fullscreen_800x600_cb);

    fullscreen_1024x800_button = new puOneShot(10, 10, "Fullscreen 1024x800");
    fullscreen_1024x800_button->setSize(buttonWidth, buttonHeight);
    fullscreen_1024x800_button->setUserData(this);
    fullscreen_1024x800_button->setCallback(fullscreen_1024x800_cb);

    fullscreen_1280x1024_button = new puOneShot(10, 10, "Fullscreen 1280x1024");
    fullscreen_1280x1024_button->setSize(buttonWidth, buttonHeight);
    fullscreen_1280x1024_button->setUserData(this);
    fullscreen_1280x1024_button->setCallback(fullscreen_1280x1024_cb);

    ok_button = new puOneShot(10, 10, "OK");
    ok_button->setSize(buttonWidth, buttonHeight);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);
	resize();
    close();
}

DlgOptions::~DlgOptions()
{
    puDeleteObject(frame);
    puDeleteObject(window_button);
    puDeleteObject(fullscreen_640x480_button);
    puDeleteObject(fullscreen_800x600_button);
    puDeleteObject(fullscreen_1024x800_button);
    puDeleteObject(fullscreen_1280x1024_button);
    puDeleteObject(ok_button);
}

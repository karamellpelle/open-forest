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
#include "dlgMenu.h"
#include "dlgStack.h"
#include "dlgHost1.h"
#include "dlgHost2.h"
#include "dlgJoin1.h"
#include "dlgJoin2.h"
#include "dlgOptions.h"


void DlgMenu::reshape(int x, int y, int width, int height)
{
    const int padding = 10;
    const int buttonWidth = 200;
    const int buttonHeight = 20;

    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    select_orienteer_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 2*(buttonHeight + padding)));
    start_singleplayer_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 1*(buttonHeight + padding)));
    join_multiplayer_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 0*(buttonHeight + padding)));
    host_multiplayer_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -1*(buttonHeight + padding)));
    view_results_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -2*(buttonHeight + padding)));
    quit_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -3*(buttonHeight + padding)));
}

void DlgMenu::select_orienteer_cb(puObject *btn)
{
    DlgMenu* dlgMenu = (DlgMenu*)btn->getUserData();
    DlgSelect* dlgSelect = new DlgSelect(dlgMenu, 50, 50, &DlgBase::pop_cb);
    DlgStack::instance().push(dlgSelect);
}

void DlgMenu::start_singleplayer_cb(puObject *btn)
{
    DlgMenu* dlgMenu = (DlgMenu*)btn->getUserData();
    DlgSingle* dlgSingle = new DlgSingle(dlgMenu, 50, 50, 
					 &MainWindow::view_results_cb,
					 &DlgBase::pop_cb);
    DlgStack::instance().push(dlgSingle);
}

void DlgMenu::join_multiplayer_cb(puObject *btn)
{
    DlgMenu* dlgMenu = (DlgMenu*)btn->getUserData();
    DlgJoin1* dlgJoin1 = new DlgJoin1(dlgMenu, 50, 50, 
				      &MainWindow::join1_connect_cb, 
				      &DlgBase::pop_cb);
    Database& db = Database::instance();
    db.addObserver(dlgJoin1);
    db.updateServerObservers(0);

    DlgStack::instance().push(dlgJoin1);
}

void DlgMenu::host_multiplayer_cb(puObject *btn)
{
    DlgMenu* dlgMenu = (DlgMenu*)btn->getUserData();
    DlgHost1* dlgHost1 = new DlgHost1(dlgMenu, 50, 50, 
				      &MainWindow::host1_start_server_cb,
				      &DlgBase::pop_cb);
    DlgStack::instance().push(dlgHost1);
}

void DlgMenu::view_results_cb(puObject *btn)
{
    DlgMenu* dlgMenu = (DlgMenu*)btn->getUserData();
	DlgOptions* dlgOptions = new DlgOptions(dlgMenu, 50, 50, &DlgBase::pop_cb);
    DlgStack::instance().push(dlgOptions);
}

void DlgMenu::quit_cb(puObject *btn)
{
    DlgMenu* dlgMenu = (DlgMenu*)btn->getUserData();
    DlgConfirm::confirm(dlgMenu, "Quit game?", 
			"Quit", &MainWindow::quit_ok_cb,
			"Cancel", &DlgBase::pop_cb);
}

void DlgMenu::init()
{
    int width = 600;
    int height = 400;
    const int buttonWidth = 200;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Main menu");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    select_orienteer_button = new puOneShot(10, 10, "Select orienteer");
    select_orienteer_button->setSize(buttonWidth, buttonHeight);
    select_orienteer_button->setUserData(this);
    select_orienteer_button->setCallback(select_orienteer_cb);

    start_singleplayer_button = new puOneShot(10, 10, "Start singleplayer game");
    start_singleplayer_button->setSize(buttonWidth, buttonHeight);
    start_singleplayer_button->setUserData(this);
    start_singleplayer_button->setCallback(start_singleplayer_cb);

    join_multiplayer_button = new puOneShot(10, 10, "Join multiplayer game");
    join_multiplayer_button->setSize(buttonWidth, buttonHeight);
    join_multiplayer_button->setUserData(this);
    join_multiplayer_button->setCallback(join_multiplayer_cb);

    host_multiplayer_button = new puOneShot(10, 10, "Host multiplayer game");
    host_multiplayer_button->setSize(buttonWidth, buttonHeight);
    host_multiplayer_button->setUserData(this);
    host_multiplayer_button->setCallback(host_multiplayer_cb);

    view_results_button = new puOneShot(10, 10, "Options");
    view_results_button->setSize(buttonWidth, buttonHeight);
    view_results_button->setUserData(this);
    view_results_button->setCallback(view_results_cb);

    quit_button = new puOneShot(10, 10, "Quit");
    quit_button->setSize(buttonWidth, buttonHeight);
    quit_button->setUserData(this);
    quit_button->setCallback(quit_cb);

    close();
#if 0
    reveal();
#endif
}

DlgMenu::~DlgMenu()
{
    puDeleteObject(frame);
    puDeleteObject(select_orienteer_button);
    puDeleteObject(start_singleplayer_button);
    puDeleteObject(join_multiplayer_button);
    puDeleteObject(host_multiplayer_button);
    puDeleteObject(view_results_button);
    puDeleteObject(quit_button);
}

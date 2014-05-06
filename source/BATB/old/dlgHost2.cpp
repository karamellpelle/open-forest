#include <plib/pu.h>

#include "dlgCommon.h"
#include "dlgHost2.h"


void DlgHost2::init(puCallback startrace_cb, puCallback stoprace_cb, puCallback stopserver_cb)
{
    int width = 600;
    int height = 400;
    int bw = int(buttonWidth*1.5);

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Hosting game");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    Database& db = Database::instance();

    dlgEvent = new DlgEvent(this, 10, 50, 250, 300);
    db.addObserver(dlgEvent);
    db.updateEventObservers(0);

    dlgPlayers = new DlgPlayers(this, 10, 50, 250, 300, true);

    start_race_button = new puOneShot(10, 10, "Start race");
    start_race_button->setSize(bw, buttonHeight);
    start_race_button->setUserData(this);
    start_race_button->setCallback(startRace_cb);
    start_race_cb = startrace_cb;

    stop_race_button = new puOneShot(100, 10, "Stop race");
    stop_race_button->setSize(bw, buttonHeight);
    stop_race_button->setUserData(this);
    stop_race_button->setCallback(stopRace_cb);
    stop_race_cb = stoprace_cb;

    stop_server_button = new puOneShot(100, 10, "Stop server");
    stop_server_button->setSize(bw, buttonHeight);
    stop_server_button->setUserData(this);
    stop_server_button->setCallback(stopServer_cb);
    stop_server_cb = stopserver_cb;

    start_race_button->activate();
    stop_race_button->greyOut();
    stop_server_button->activate();

    resize();

    close();
}

void DlgHost2::startRace_cb(puObject *button)
{
    DlgHost2* dlgHost2 = (DlgHost2*)button->getUserData();
    dlgHost2->start_race_cb(dlgHost2);
}

void DlgHost2::stopRace_cb(puObject *button)
{
    DlgHost2* dlgHost2 = (DlgHost2*)button->getUserData();
    dlgHost2->stop_race_cb(dlgHost2);
}

void DlgHost2::stopServer_cb(puObject *button)
{
    DlgHost2* dlgHost2 = (DlgHost2*)button->getUserData();
    dlgHost2->stop_server_cb(dlgHost2);
}

void DlgHost2::reshape(int x, int y, int width, int height)
{
    int bw = int(buttonWidth*1.5);
    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    dlgEvent->reshape(int(2*padding), int(50+0.5*padding),
		      int(0.4*width-2.5*padding), int(height-50-30-padding));

    dlgPlayers->reshape(int(0.4*width+0.5*padding), int(50+0.5*padding),
		       int(0.6*width-2.5*padding), int(height-50-30-padding));

    start_race_button->setPosition(int(width/2 - 0.5*padding - 1.5*bw), 10);
    stop_race_button->setPosition(int(width/2 - 0.5*bw), 10);
    stop_server_button->setPosition(int(width/2 + 0.5*padding + 0.5*bw), 10);
}

DlgHost2::~DlgHost2()
{
    Database& db = Database::instance();
    db.removeObserver(dlgEvent);

    puDeleteObject(frame);
    puDeleteObject(dlgPlayers);
    puDeleteObject(start_race_button);
    puDeleteObject(stop_race_button);
    puDeleteObject(stop_server_button);
}

int DlgHost2::update(char** clientsList)
{
    return dlgPlayers->update(clientsList);
}

void DlgHost2::setRaceOn(bool race_on)
{
    raceOn = race_on;

    if (raceOn)
    {
        dlgEvent->greyOut();
        start_race_button->greyOut();
        stop_race_button->activate();
        stop_server_button->greyOut();
    }
    else
    {
        dlgEvent->activate();
        start_race_button->activate();
        stop_race_button->greyOut();
        stop_server_button->activate();
    }
}


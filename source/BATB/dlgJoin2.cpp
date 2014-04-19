#include <plib/pu.h>

#include "dlgCommon.h"
#include "dlgJoin2.h"


void DlgJoin2::init(puCallback quit_cb, puCallback disconnect_cb)
{
    int width = 600;
    int height = 400;
    int bw = int(buttonWidth*1.5);

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Game controls");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    dlgPlayers = new DlgPlayers(this, 10, 50, 250, 300, false);

    quit_button = new puOneShot(10, 10, "Quit");
    quit_button->setSize(bw, buttonHeight);
    quit_button->setUserData(this);
    quit_button->setCallback(quit_cb);
    quit_cb_ = quit_cb;

    disconnect_button = new puOneShot(100, 10, "Disconnect");
    disconnect_button->setSize(bw, buttonHeight);
    disconnect_button->setUserData(this);
    disconnect_button->setCallback(disconnect_cb);
    disconnect_cb_ = disconnect_cb;

    quit_button->greyOut();
    disconnect_button->activate();

    resize();

    close();
}

void DlgJoin2::quit_cb(puObject *button)
{
    DlgJoin2* dlgJoin2 = (DlgJoin2*)button->getUserData();
    dlgJoin2->quit_button->greyOut();
    dlgJoin2->disconnect_button->activate();
    dlgJoin2->quit_cb_(button);
}

void DlgJoin2::disconnect_cb(puObject *button)
{
    DlgJoin2* dlgJoin2 = (DlgJoin2*)button->getUserData();
    dlgJoin2->quit_button->activate();
    dlgJoin2->disconnect_button->greyOut();
    dlgJoin2->disconnect_cb_(dlgJoin2);
}

void DlgJoin2::reshape(int x, int y, int width, int height)
{
    int bw = int(buttonWidth*1.5);
    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    dlgPlayers->reshape(int(2*padding), int(50+0.5*padding),
			int(width-4*padding), int(height-50-30-padding));

    quit_button->setPosition(int(width/2 - 0.5*padding - 1.5*bw), 10);
    disconnect_button->setPosition(int(width/2 - 0.5*bw), 10);
}

DlgJoin2::~DlgJoin2()
{
    puDeleteObject(frame);
    puDeleteObject(dlgPlayers);
    puDeleteObject(quit_button);
    puDeleteObject(disconnect_button);
}

void DlgJoin2::setRaceOn(bool race_on)
{
    raceOn = race_on;

    if (raceOn)
    {
	quit_button->activate();
	disconnect_button->greyOut();
    }
    else
    {
	quit_button->greyOut();
	disconnect_button->activate();
    }
}

int DlgJoin2::update(char** clientsList)
{
    return dlgPlayers->update(clientsList);
}

#include <iostream>

#include <plib/pu.h>

#include "database.h"
#include "dlgCommon.h"
#include "dlgJoin1.h"



void DlgJoin1::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 600;
    int height = 400;
    int bw = int(1.5*buttonWidth);

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Join multiplayer game");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    hostName = new puaComboBox(width/2, height/2+15, 
			      width/2+width/2-10, height/2+15+20, 
			      NULL);
    hostName->setLabel("Host name:");
    hostName->setLabelPlace(PUPLACE_CENTERED_LEFT);
    hostName->setUserData(this);
    hostName->setCallback(hostname_cb);

    portNumber = new puInput(0, 0, 0, 0);
    portNumber->setLabel("Port number:");
    portNumber->setLabelPlace(PUPLACE_CENTERED_LEFT);

    portNumber->setValue("1234");

    connect_button = new puOneShot(10, 10, "Connect");
    connect_button->setSize(bw, buttonHeight);
    connect_button->setUserData(this);
    connect_button->setCallback(ok_cb);

    cancel_button = new puOneShot(10, 10, "Cancel");
    cancel_button->setSize(bw, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    resize();

    close();
}

void DlgJoin1::hostname_cb(puObject *hostname)
{
    DlgJoin1* dlgJoin1 = (DlgJoin1*)hostname->getUserData();
    std::cout << "Hostname: " << dlgJoin1->getHostName() << "\n";
}

void DlgJoin1::reshape(int x, int y, int width, int height)
{
    int bw = int(1.5*buttonWidth);
    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    hostName->setPosition(width/2, height/2+15);
    hostName->setSize(width/2-2*padding, 20);
    
    portNumber->setPosition(width/2, height/2-15);
    portNumber->setSize(width/2-2*padding, 20);
    
    connect_button->setPosition(int(width/2 - 0.5*padding - bw), 10);
    cancel_button->setPosition(int(width/2 + 0.5*padding), 10);
}

DlgJoin1::~DlgJoin1()
{
    puDeleteObject(frame);
    puDeleteObject(hostName);
    puDeleteObject(portNumber);
    puDeleteObject(connect_button);
    puDeleteObject(cancel_button);
    Database& db = Database::instance();
    db.removeObserver(this);
}

int DlgJoin1::updateServers(void* callData) const
{
    Database& db = Database::instance();
    char** serverList = db.getServers();
    hostName->newList(serverList);
    return 0;
}

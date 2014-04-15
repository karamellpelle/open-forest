#include <iostream>
#include <algorithm>

#include "database.h"
#include "dlgCommon.h"
#include "dlgPlayers.h"
#include "dlgStack.h"
#include "mainWindow.h"


void DlgPlayers::reshape(int x, int y, int width, int height)
{
    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    // Resize and position the slider
    slider->setPosition(int(width-30+padding/2), int(40+0.5*padding));
    slider->setSize(20, height - 40 - 20 - padding);

    // Resize the list box
    list_box->setPosition(padding, int(40+0.5*padding));
    list_box->setSize(int(width - 30 - padding/2), int(height - 40 - 20 - padding));

    if (enableKick)
    {
	kick_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    }
}

void DlgPlayers::kick_cb(puObject* kick_button)
{
    DlgPlayers* dlgPlayers = (DlgPlayers*)kick_button->getUserData();
    int si = dlgPlayers->list_box->getIntegerValue();
    if (si == -1)
    {
	DlgInformation::info(dlgPlayers->getParent(), "No player selected");
    }
    else
    {
	DlgConfirm::confirm(dlgPlayers->getParent(), "Kick the selected players?", 
			    "Kick", &MainWindow::kick_ok_cb, 
			    "Cancel", &DlgBase::pop_cb);
    }
}

void DlgPlayers::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgPlayers::handle_select(puObject* list_box)
{
    DlgPlayers* dlgPlayers = (DlgPlayers*)list_box->getUserData();
    int si = list_box->getIntegerValue();
    dlgPlayers->setSelectedIndex(si);
}

void DlgPlayers::init(int width, int height)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, NULL);
    list_box->setLabel("Players");
    list_box->setLabelPlace(PUPLACE_TOP_CENTERED);
    list_box->setStyle(-PUSTYLE_SMALL_SHADED);
    list_box->setUserData(this);
    list_box->setCallback(handle_select);

    slider->setUserData(list_box);
    slider->setCallback(handle_slider);

    if (enableKick)
    {
	kick_button = new puOneShot(10, 10, "Kick");
	kick_button->setSize(buttonWidth, buttonHeight);
	kick_button->makeReturnDefault (TRUE);
	kick_button->setUserData(this);
	kick_button->setCallback(kick_cb);
    }

    close(); // close the group
#if 0
    reveal();
#endif
}

DlgPlayers::~DlgPlayers()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    if (enableKick)
    {
	puDeleteObject(kick_button);
    }
}

int DlgPlayers::update(char** clientsList)
{
    list_box->newList(clientsList);
    return 0;
}

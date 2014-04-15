#define NOMINMAX

#include <iostream>
#include <algorithm>

#include "database.h"

#include "mainWindow.h"
#include "dlgCommon.h"
#include "dlgResult.h"
#include "dlgStack.h"


void DlgResult::reshape(int x, int y, int width, int height)
{
    setPosition(x, y);
    
    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    // Resize and position the slider
    slider->setPosition(int(width-30+padding/2), int(40+0.5*padding));
    slider->setSize(20, height - 40 - 20 - padding);

    // Resize the list box
    list_box->setPosition(int(padding), int(40+0.5*padding));
    list_box->setSize(int(width - 30 - padding/2), int(height - 40 - 20 - padding));

    new_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    delete_button->setPosition(int(width/2 + 0.5*padding), padding);
}

void DlgResult::new_cb(puObject* new_button)
{
}

void DlgResult::delete_ok_cb(puObject *)
{
    try
    {
	Database& db = Database::instance();
	db.removeRun(db.getSelectedRun());
	db.updateRunsView();
	db.adjustSelectedRun();
	db.updateRunObservers(0);

	DlgBase* dlg = DlgStack::instance().pop();
	puDeleteObject(dlg);
    }
    catch (const NotFound& ex)
    {
	DlgBase* dlg = DlgStack::instance().top();
	DlgInformation::info(dlg, ex.getMessage());
    }
}

void DlgResult::delete_cb(puObject *delete_button)
{
    DlgResult* dlgResult = (DlgResult*)delete_button->getUserData();
    DlgConfirm::confirm(dlgResult->getParent(), "Really delete?", 
			"Delete", delete_ok_cb, 
			"Cancel", &DlgBase::pop_cb);
}

void DlgResult::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val ;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgResult::handle_select(puObject* l_box)
{
    int selectedIndex = l_box->getIntegerValue();
    Database& db = Database::instance();
    db.setSelectedRunIndex(selectedIndex);
    db.updateRunObservers(0);
}

void DlgResult::init(int width, int height)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, NULL);
    list_box->setLabel("Runs");
    list_box->setLabelPlace(PUPLACE_TOP_CENTERED);
    list_box->setStyle(-PUSTYLE_SMALL_SHADED);
    list_box->setUserData(this);
    list_box->setCallback(handle_select);

    slider->setUserData(list_box);
    slider->setCallback(handle_slider);

    new_button = new puOneShot(10, 10, "New");
    new_button->setSize(buttonWidth, buttonHeight);
    new_button->makeReturnDefault (TRUE);
    new_button->setUserData(this);
    new_button->setCallback(&MainWindow::single_start_cb);
    delete_button = new puOneShot(60, 10, "Delete");
    delete_button->setSize(buttonWidth, buttonHeight);
    delete_button->makeReturnDefault (TRUE);
    delete_button->setUserData(this);
    delete_button->setCallback(delete_cb);

    resize();

    close(); // close the group
}

DlgResult::~DlgResult()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    puDeleteObject(new_button);
    puDeleteObject(delete_button);
}

int DlgResult::updateRuns(void* callData) const
{
    Database& db = Database::instance();
    char** runList = db.getSelectedRuns();
    const int selectedIndex = db.getSelectedRunIndex();
    
    list_box->newList(runList);
    list_box->setValue(selectedIndex);

    if (selectedIndex == -1)
	delete_button->greyOut();
    else
	delete_button->activate();
    return 0;
}

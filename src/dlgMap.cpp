#include <iostream>

#include <plib/pu.h>

#include "database.h"
#include "map.h"

#include "dlgCommon.h"
#include "dlgMap.h"
#include "dlgStack.h"


class DlgNewMap : public DlgBase
{
public:
    std::string getName() { _name = name->getStringValue(); return _name; }
    int getContourInterval() const { return contourInterval; }
    void reshape(int x, int y, int width, int height);

    DlgNewMap(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb, cancel_cb);
    }

    ~DlgNewMap();
    void print() { std::cout << "DlgNewMap\n"; }

private:
    puInput*  name;
    std::string _name;

    puSlider* contourIntervalSlider;
    char contourIntervalLegend[32];
    int contourInterval;
    static void contourInterval_cb(puObject* slider);
    static int contourIntervalMin;
    static int contourIntervalMax;
    static int contourIntervalDefault;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);
};

int DlgNewMap::contourIntervalMin = 1;
int DlgNewMap::contourIntervalMax = 20;
int DlgNewMap::contourIntervalDefault = 5;

void DlgNewMap::reshape(int x, int y, int width, int height)
{
    int widgetHeight = 20;

    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(int(width/2), int(height/2 + 2*(widgetHeight + padding)));
    name->setSize(int(width/2-2*padding), widgetHeight);

    contourIntervalSlider->setPosition(int(width/2), int(height/2 + 1*(widgetHeight + padding)));
    contourIntervalSlider->setSize(int(width/2-2*padding), widgetHeight);

    ok_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    cancel_button->setPosition(int(width/2 + 0.5*padding), padding);
}

void DlgNewMap::contourInterval_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewMap* dlgNewMap = (DlgNewMap*)slider->getUserData();
    dlgNewMap->contourInterval = 
	int(value*(contourIntervalMax - contourIntervalMin) + 
	    contourIntervalMin);
    sprintf(dlgNewMap->contourIntervalLegend, "%d", dlgNewMap->contourInterval);
}

void DlgNewMap::setDefaultValues()
{
    contourInterval = contourIntervalDefault;
    float sliderValue = float(contourInterval - contourIntervalMin)/
	(contourIntervalMax - contourIntervalMin);
    contourIntervalSlider->setValue(sliderValue);
}

void DlgNewMap::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 300;
    int height = 300;
    int widgetXPos = 190;
    int widgetYPos = height-70;
    int widgetYPosDelta = 30;
    int widgetHeight = 20;
    int widgetWidth = 100;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Create a new map");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    int widgetY = widgetYPos - 0*widgetYPosDelta;
    name = new puInput(widgetXPos, widgetY, 
		       widgetXPos+widgetWidth, widgetY+widgetHeight);
    name->setLabel("Name:");
    name->setLabelPlace(PUPLACE_CENTERED_LEFT);

    contourIntervalSlider = new puSlider(widgetXPos, widgetYPos - 1*widgetYPosDelta, 
					 widgetWidth, false, widgetHeight);
    contourIntervalSlider->setLabel("Contour interval:");
    contourIntervalSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    contourIntervalSlider->setCallback(contourInterval_cb);
    contourIntervalSlider->setUserData(this);
    contourIntervalSlider->setLegend(contourIntervalLegend);

    ok_button = new puOneShot(10, 10, "OK");
    ok_button->setSize(80, 30);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);

    cancel_button = new puOneShot(100, 10, "Cancel");
    cancel_button->setSize(80, 30);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    resize();

    close();

    setDefaultValues();
    contourInterval_cb(contourIntervalSlider);
}


void DlgMap::reshape(int x, int y, int width, int height)
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

    new_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    delete_button->setPosition(int(width/2 + 0.5*padding), padding);
}

DlgNewMap::~DlgNewMap()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(contourIntervalSlider);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}

void DlgMap::new_ok_cb(puObject *)
{
    DlgNewMap* dlg = (DlgNewMap*)DlgStack::instance().top();
   
    Database& db = Database::instance();
    std::string name = dlg->getName();
    unsigned int sz = name.size();
    std::string::size_type i = name.find_first_of(' ');
    if (sz == 0)
    {
	DlgInformation::info(dlg, "You must give name for the map");
    }
    else if (0 <= i && i < sz)
    {
	DlgInformation::info(dlg, "Name may not contain space characters");
    }
    else 
    {
	try
	{
	    Terrain* terrain = db.getSelectedTerrain();
	    try
	    {
		int contourInterval = dlg->getContourInterval();
		Map* map = new Map(name, terrain->id(), contourInterval);
		db.addMap(map);
		db.updateMapsView();
		db.setSelectedMap(map);
		db.updateMapObservers(0);
		dlg = (DlgNewMap*)DlgStack::instance().pop();
		puDeleteObject(dlg);
	    }
	    catch (const Exists& ex)
	    {
		delete terrain;
		DlgInformation::info(dlg, ex.getMessage());
	    }
	}
	catch (const NotFound& ex)
	{
	    DlgInformation::info(dlg, ex.getMessage());
	}
    }
}

void DlgMap::new_cb(puObject* new_button)
{
    DlgMap* dlgMap = (DlgMap*)new_button->getUserData();
    DlgBase* dlgNewMap = new DlgNewMap(dlgMap, 100, 100, 
				       new_ok_cb, &DlgBase::pop_cb);
    int x, y, width, height;
    DlgBase* parent = dlgMap->getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgNewMap->reshape(x, y, width, height);

    DlgStack::instance().push(dlgNewMap);
}

void DlgMap::delete_ok_cb(puObject *)
{
    try
    {
	Database& db = Database::instance();
	db.removeMap(db.getSelectedMap());
	db.updateMapsView();
	db.adjustSelectedMap();
	db.updateMapObservers(0);

	db.ensureMinimalDB();

	puGroup* dlg = DlgStack::instance().pop();
	puDeleteObject(dlg);
    } 
    catch (const NotFound& ex)
    {
	DlgBase* dlg = DlgStack::instance().top();
	DlgInformation::info(dlg, ex.getMessage());
    }
}

void DlgMap::delete_cancel_cb(puObject *)
{
    puGroup* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
}

void DlgMap::delete_cb(puObject *delete_button)
{
    DlgMap* dlgMap = (DlgMap*)delete_button->getUserData();
    DlgConfirm::confirm(dlgMap->getParent(), "Really delete?", 
			"Delete", delete_ok_cb, 
			"Cancel", &DlgBase::pop_cb);
}

void DlgMap::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val ;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgMap::handle_select(puObject* l_box)
{
    int selectedIndex = l_box->getIntegerValue();
    Database& db = Database::instance();
    db.setSelectedMapIndex(selectedIndex);
    db.updateMapObservers(0);
}

void DlgMap::init(int width, int height, char** items)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, items);
    list_box->setLabel("Maps");
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
    new_button->setCallback(new_cb);

    delete_button = new puOneShot(60, 10, "Delete");
    delete_button->setSize(buttonWidth, buttonHeight);
    delete_button->makeReturnDefault (TRUE);
    delete_button->setUserData(this);
    delete_button->setCallback(delete_cb);

    close(); // close the group
}

DlgMap::~DlgMap()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    puDeleteObject(new_button);
    puDeleteObject(delete_button);
}

int DlgMap::updateMaps(void* callData) const
{
    Database& db = Database::instance();
    char** mapList = db.getSelectedMaps();
    const int selectedIndex = db.getSelectedMapIndex();
    
    list_box->newList(mapList);
    list_box->setValue(selectedIndex);

    if (selectedIndex == -1)
	delete_button->greyOut();
    else
	delete_button->activate();
    return 0;
}

int DlgMap::updateTerrains(void* callData) const
{
    Database& db = Database::instance();
    const int selectedIndex = db.getSelectedTerrainIndex();
    
    if (selectedIndex == -1)
	new_button->greyOut();
    else
	new_button->activate();
    return 0;
}

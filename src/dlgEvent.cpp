#include <sstream>

#include <plib/pu.h>

#include "database.h"

#include "dlgCommon.h"
#include "dlgTerrain.h"
#include "dlgMap.h"
#include "dlgCourse.h"
#include "dlgEvent.h"
#include "dlgStack.h"


class DlgNewEvent : public DlgBase
{
public:
    std::string getName() { _name = name->getStringValue(); return _name; }
	bool getNight() { return (bool)(night->getIntegerValue()); }
    void reshape(int x, int y, int width, int height);

    DlgNewEvent(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
        init(ok_cb, cancel_cb);
    }

    ~DlgNewEvent();
    void print() { std::cout << "DlgNewEvent\n"; }

private:
    puInput*  name;
    std::string _name;

	puButton* night;

    DlgTerrain* dlgTerrain;
    DlgMap* dlgMap;
    DlgCourse* dlgCourse;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);
};

void DlgNewEvent::reshape(int x, int y, int width, int height)
{
    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(60, height-60);
    name->setSize(int(width/2-60-0.5*padding), 20);
    
    night->setPosition(int(width/2+0.5*padding), height-60);
    night->setSize(20, 20);
    
    dlgTerrain->reshape(int(2*padding), int(50+0.5*padding),
			int(width/2-2.5*padding), int(height-(50+70+padding)));

    dlgMap->reshape(int(width/2+0.5*padding), int(height/2+0.5*padding),
		    int(width/2-2.5*padding), int(height/2-(70+padding)));

    dlgCourse->reshape(int(width/2+0.5*padding), int(50+0.5*padding),
		       int(width/2-2.5*padding), int(height/2-(50+padding)));

    ok_button->setPosition(width/2-5-80, 10);
    cancel_button->setPosition(width/2+5, 10);
}

DlgNewEvent::~DlgNewEvent()
{
    Database& db = Database::instance();
    db.removeObserver(dlgTerrain);
    db.removeObserver(dlgMap);
    db.removeObserver(dlgCourse);

    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(dlgTerrain);
    puDeleteObject(dlgMap);
    puDeleteObject(dlgCourse);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}

void DlgNewEvent::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 600;
    int height = 400;
    int widgetXPos = 190;
    int widgetYPos = height-70;
    int widgetYPosDelta = 30;
    int widgetHeight = 20;
    int widgetWidth = 100;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Create a new event");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    int widgetY = widgetYPos - 0*widgetYPosDelta;
    name = new puInput(widgetXPos, widgetY, 
		       widgetXPos+widgetWidth, widgetY+widgetHeight);
    name->setLabel("Name:");
    name->setLabelPlace(PUPLACE_CENTERED_LEFT);

	night = new puButton(0, 0, "");
	night->setLabel("Night mode");
	//night->setButtonType(PUBUTTON_CIRCLE);
	night->setButtonType(PUBUTTON_XCHECK);

    Database& db = Database::instance();

    dlgTerrain = new DlgTerrain(this, 10, 50, 250, 300);
    db.addObserver(dlgTerrain);

    dlgMap = new DlgMap(this, 260, 180, 500, 300);
    db.addObserver(dlgMap);

    dlgCourse = new DlgCourse(this, 260, 50, 500, 170);
    db.addObserver(dlgCourse);

    db.updateTerrainObservers(0);
    db.updateMapObservers(0);
    db.updateCourseObservers(0);

    ok_button = new puOneShot(10, 10, "OK");
    ok_button->setSize(buttonWidth, buttonHeight);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);

    cancel_button = new puOneShot(100, 10, "Cancel");
    cancel_button->setSize(buttonWidth, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    close();
}

class DlgQuickCreate : public DlgBase
{
 public:
    void reshape(int x, int y, int width, int height);

    DlgQuickCreate(DlgBase* parent, int x, int y)
	: DlgBase(parent, x, y)
    {
	init();
    }
    virtual ~DlgQuickCreate();
    void print() { std::cout << "DlgQuickCreate\n"; }

 private:
    puOneShot* quick_create1_button;
    puOneShot* quick_create2_button;
    puOneShot* quick_create3_button;
    puOneShot* manual_create_button;
    puOneShot* cancel_button;

    static void quick_create1_cb(puObject *btn);
    static void quick_create2_cb(puObject *dlg);
    static void quick_create3_cb(puObject *dlg);
    static void manual_create_cb(puObject *dlg);
    static void cancel_cb(puObject *btn);

    static void new_ok_cb(puObject *);

    void init();
};

DlgQuickCreate::~DlgQuickCreate()
{
    puDeleteObject(quick_create1_button);
    puDeleteObject(quick_create2_button);
    puDeleteObject(quick_create3_button);
    puDeleteObject(manual_create_button);
    puDeleteObject(cancel_button);
}

void DlgQuickCreate::init()
{
    int width = 600;
    int height = 400;
    const int buttonWidth = 200;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Create new event");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    quick_create1_button = new puOneShot(10, 10, "Quick create short");
    quick_create1_button->setSize(buttonWidth, buttonHeight);
    quick_create1_button->setUserData(this);
    quick_create1_button->setCallback(quick_create1_cb);

    quick_create2_button = new puOneShot(10, 10, "Quick create medium");
    quick_create2_button->setSize(buttonWidth, buttonHeight);
    quick_create2_button->setUserData(this);
    quick_create2_button->setCallback(quick_create2_cb);

    quick_create3_button = new puOneShot(10, 10, "Quick create long");
    quick_create3_button->setSize(buttonWidth, buttonHeight);
    quick_create3_button->setUserData(this);
    quick_create3_button->setCallback(quick_create3_cb);

    manual_create_button = new puOneShot(10, 10, "Manual create");
    manual_create_button->setSize(buttonWidth, buttonHeight);
    manual_create_button->setUserData(this);
    manual_create_button->setCallback(manual_create_cb);

    cancel_button = new puOneShot(10, 10, "Cancel");
    cancel_button->setSize(buttonWidth, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    close();
}

void DlgQuickCreate::reshape(int x, int y, int width, int height)
{
    const int padding = 10;
    const int buttonWidth = 200;
    const int buttonHeight = 20;

    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    quick_create1_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 2*(buttonHeight + padding)));
    quick_create2_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 1*(buttonHeight + padding)));
    quick_create3_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + 0*(buttonHeight + padding)));
    manual_create_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -1*(buttonHeight + padding)));
    cancel_button->setPosition(int(width/2 - buttonWidth/2), int(height/2 + -2*(buttonHeight + padding)));
}

void DlgQuickCreate::quick_create1_cb(puObject* new_button)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
    Database& db = Database::instance();
    db.quick_create_short();
}

void DlgQuickCreate::quick_create2_cb(puObject* new_button)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
    Database& db = Database::instance();
    db.quick_create_medium();
}

void DlgQuickCreate::quick_create3_cb(puObject* new_button)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
    Database& db = Database::instance();
    db.quick_create_long();
}

void DlgQuickCreate::new_ok_cb(puObject *)
{
    DlgNewEvent* dlg = (DlgNewEvent*)DlgStack::instance().top();
    std::string name = dlg->getName();
	bool night = dlg->getNight();
    unsigned int sz = name.size();
    std::string::size_type i = name.find_first_of(' ');
    if (sz == 0)
    {
        DlgInformation::info(dlg, "You must give name for the event");
    }
    else if (0 <= i && i < sz)
    {
        DlgInformation::info(dlg, "Name may not contain space characters");
    }
    else
    {
        try
        {
            Database& db = Database::instance();
            Map* map = db.getSelectedMap();
            Course* course = db.getSelectedCourse();
			
            Event* event = new Event(name, map->id(), course->id(), night, false);
            try
            {
            	db.addEvent(event);
            	db.updateEventsView();
            	db.setSelectedEvent(event);
            	db.updateEventObservers(0);
            	DlgBase* dlgBase = DlgStack::instance().pop();
            	puDeleteObject(dlgBase);
	        }
	        catch (const Exists& ex)
	        {
            	delete event;
        	    DlgInformation::info(dlg, ex.getMessage());
	        }
	    }
	    catch (const NotFound& ex)
	    {
    	    DlgInformation::info(dlg, ex.getMessage());
    	}
    }
}

void DlgQuickCreate::manual_create_cb(puObject* new_button)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
    DlgEvent* dlgEvent = (DlgEvent*)new_button->getUserData();
    DlgBase* dlgNewEvent = new DlgNewEvent(dlgEvent, 50, 50, 
					   new_ok_cb, &DlgBase::pop_cb);

    int x, y, width, height;
    dlgEvent->getPosition(&x, &y);
    dlgEvent->getSize(&width, &height);
    dlgNewEvent->reshape(x, y, width, height);
    
    DlgStack::instance().push(dlgNewEvent);
}

void DlgQuickCreate::cancel_cb(puObject* new_button)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
}

void DlgEvent::reshape(int x, int y, int width, int height)
{
    //int bw = int(buttonWidth*0.8);
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
    //quick_create_button->setPosition(int(width/2 - 0.5*bw), padding);
    delete_button->setPosition(int(width/2 + 0.5*padding), padding);
}

void DlgEvent::new_cancel_cb(puObject *)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
}

void DlgEvent::new_cb(puObject* new_button)
{
    DlgEvent* dlgEvent = (DlgEvent*)new_button->getUserData();
    DlgBase* dlgQuickCreate = new DlgQuickCreate(dlgEvent, 50, 50);

    int x, y, width, height;
    DlgBase* parent = dlgEvent->getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgQuickCreate->reshape(x, y, width, height);
    
    DlgStack::instance().push(dlgQuickCreate);
}

void DlgEvent::delete_ok_cb(puObject *)
{
    try
    {
        Database& db = Database::instance();
	Event* event = db.getSelectedEvent();
        db.removeEvent(event);
        db.updateEventsView();
        db.adjustSelectedEvent();
        db.updateEventObservers(0);

	db.ensureMinimalDB();

        DlgBase* dlg = DlgStack::instance().pop();
        puDeleteObject(dlg);
    }
    catch (const NotFound& ex)
    {
        DlgBase* dlg = DlgStack::instance().top();
        DlgInformation::info(dlg, ex.getMessage());
    }
}

void DlgEvent::delete_cb(puObject *delete_button)
{
    DlgEvent* dlgEvent = (DlgEvent*)delete_button->getUserData();
    DlgConfirm::confirm(dlgEvent->getParent(), "Really delete?", 
                        "Delete", delete_ok_cb, 
                        "Cancel", &DlgBase::pop_cb);
}

void DlgEvent::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val ;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgEvent::handle_select(puObject* l_box)
{
    int selectedIndex = l_box->getIntegerValue();
    Database& db = Database::instance();
    db.setSelectedEventIndex(selectedIndex);
    db.updateEventObservers(0);
}

void DlgEvent::init(int width, int height)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, NULL);
    list_box->setLabel("Events");
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

DlgEvent::~DlgEvent()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    puDeleteObject(new_button);
    puDeleteObject(delete_button);
}

int DlgEvent::updateEvents(void* callData) const
{
    Database& db = Database::instance();
    char** eventList = db.getSelectedEvents();
    const int selectedIndex = db.getSelectedEventIndex();
    
    list_box->newList(eventList);
    list_box->setValue(selectedIndex);

    if (selectedIndex == -1)
        delete_button->greyOut();
    else
        delete_button->activate();
    return 0;
}

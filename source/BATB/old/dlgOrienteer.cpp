#include <plib/pu.h>

#include "database.h"

#include "dlgCommon.h"
#include "dlgOrienteer.h"
#include "dlgStack.h"


class DlgNewOrienteer : public DlgBase
{
public:
    std::string getName() { _name = name->getStringValue(); return _name; }
    void reshape(int x, int y, int width, int height);

    DlgNewOrienteer(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb, cancel_cb);
    }

    virtual ~DlgNewOrienteer();
    void print() { std::cout << "DlgNewOrienteer\n"; }

private:
    puInput* name;
    std::string _name;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);

};

void DlgNewOrienteer::reshape(int x, int y, int width, int height)
{
    int widgetHeight = 20;

    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(int(width/2), int(height/2 + 2*(widgetHeight + padding)));
    name->setSize(int(width/2-2*padding), widgetHeight);
    
    ok_button->setPosition(width/2-5-80, 10);
    cancel_button->setPosition(width/2+5, 10);
}

void DlgNewOrienteer::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 600;
    int height = 400;
    int widgetXPos = 190;
    int widgetYPos = height-70;
    int widgetYPosDelta = 30;
    int widgetHeight = 20;
    int widgetWidth = 100;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Create a new orienteer");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    int widgetY = widgetYPos - 0*widgetYPosDelta;
    name = new puInput(widgetXPos, widgetY, 
		       widgetXPos+widgetWidth, widgetY+widgetHeight);
    name->setLabel("Name:");
    name->setLabelPlace(PUPLACE_CENTERED_LEFT);

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

DlgNewOrienteer::~DlgNewOrienteer()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}

void DlgOrienteer::reshape(int x, int y, int width, int height)
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

void DlgOrienteer::new_ok_cb(puObject *)
{
    DlgNewOrienteer* dlg = (DlgNewOrienteer*)DlgStack::instance().top();
    
    Database& db = Database::instance();
    std::string name = dlg->getName();
    unsigned int sz = name.size();
    std::string::size_type i = name.find_first_of(' ');
    if (sz == 0)
    {
	DlgInformation::info(dlg, "You must give name for the orienteer");
    }
    else if (0 <= i && i < sz)
    {
	DlgInformation::info(dlg, "Name may not contain space characters");
    }
    else 
    {
	Orienteer* orienteer = new Orienteer(name);
	try
	{
	    db.addOrienteer(orienteer);
	    db.updateOrienteersView();
	    db.setSelectedOrienteer(orienteer);
	    db.updateOrienteerObservers(0);
	    dlg = (DlgNewOrienteer*)DlgStack::instance().pop();
	    puDeleteObject(dlg);
	}
	catch (const Exists& ex)
	{
	    delete orienteer;
	    DlgInformation::info(dlg, ex.getMessage());
	}
    }
}

void DlgOrienteer::new_cb(puObject* new_button)
{
    DlgOrienteer* dlgOrienteer = (DlgOrienteer*)new_button->getUserData();
    DlgBase* dlgNewOrienteer = new DlgNewOrienteer(dlgOrienteer, 50, 50, 
						   new_ok_cb, &DlgBase::pop_cb);
    int x, y, width, height;
    DlgBase* parent = dlgOrienteer->getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgNewOrienteer->reshape(x, y, width, height);

    DlgStack::instance().push(dlgNewOrienteer);
}

void DlgOrienteer::delete_ok_cb(puObject *)
{
    try
    {
	Database& db = Database::instance();
	db.removeOrienteer(db.getSelectedOrienteer());
	db.updateOrienteersView();
	db.adjustSelectedOrienteer();
	db.updateOrienteerObservers(0);

	DlgBase* dlg = DlgStack::instance().pop();
	puDeleteObject(dlg);
    }
    catch (const NotFound& ex)
    {
	DlgBase* dlg = DlgStack::instance().top();
	DlgInformation::info(dlg, ex.getMessage());
    }
}

void DlgOrienteer::delete_cb(puObject *delete_button)
{
    DlgOrienteer* dlgOrienteer = (DlgOrienteer*)delete_button->getUserData();
    DlgConfirm::confirm(dlgOrienteer->getParent(), "Really delete?", 
			"Delete", delete_ok_cb, 
			"Cancel", &DlgBase::pop_cb);
}

void DlgOrienteer::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val ;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgOrienteer::handle_select(puObject* l_box)
{
    int selectedIndex = l_box->getIntegerValue();
    Database& db = Database::instance();
    db.setSelectedOrienteerIndex(selectedIndex);
    db.updateOrienteerObservers(0);
}

void DlgOrienteer::init(int width, int height)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, NULL);
    list_box->setLabel("Orienteers");
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

DlgOrienteer::~DlgOrienteer()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    puDeleteObject(new_button);
    puDeleteObject(delete_button);
}


int DlgOrienteer::updateOrienteers(void* callData) const
{
    Database& db = Database::instance();
    char** orienteerList = db.getSelectedOrienteers();
    const int selectedIndex = db.getSelectedOrienteerIndex();
    
    list_box->newList(orienteerList);
    list_box->setValue(selectedIndex);

    if (selectedIndex == -1)
	delete_button->greyOut();
    else
	delete_button->activate();
    return 0;
}

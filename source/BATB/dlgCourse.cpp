#include <iostream>

#include <plib/pu.h>

#include "database.h"
#include "map.h"

#include "dlgCommon.h"
#include "dlgCourse.h"
#include "dlgStack.h"


class DlgNewCourse : public DlgBase
{
public:
    std::string getName() { _name = name->getStringValue(); return _name; }
    int getNumberOfControls() const { return nControls; }
    void reshape(int x, int y, int width, int height);

    DlgNewCourse(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb, cancel_cb);
    }

    ~DlgNewCourse();
    void print() { std::cout << "DlgNewCourse\n"; }

private:
    puInput*  name;
    std::string _name;

    puSlider* nControlsSlider;
    char nControlsLegend[32];
    int nControls;
    static void nControls_cb(puObject* slider);
    static int nControlsMin;
    static int nControlsMax;
    static int nControlsDefault;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);
};

int DlgNewCourse::nControlsMin = 2;
int DlgNewCourse::nControlsMax = 20;
int DlgNewCourse::nControlsDefault = 10;

void DlgNewCourse::reshape(int x, int y, int width, int height)
{
    int widgetHeight = 20;

    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(int(width/2), int(height/2 + 2*(widgetHeight + padding)));
    name->setSize(int(width/2-2*padding), widgetHeight);

    nControlsSlider->setPosition(int(width/2), int(height/2 + 1*(widgetHeight + padding)));
    nControlsSlider->setSize(int(width/2-2*padding), widgetHeight);

    ok_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    cancel_button->setPosition(int(width/2 + 0.5*padding), padding);
}

void DlgNewCourse::nControls_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewCourse* dlgNewCourse = (DlgNewCourse*)slider->getUserData();
    dlgNewCourse->nControls = 
	int(value*(nControlsMax - nControlsMin) + nControlsMin);
    sprintf(dlgNewCourse->nControlsLegend, "%d", dlgNewCourse->nControls);
}

void DlgNewCourse::setDefaultValues()
{
    nControls = nControlsDefault;
    float sliderValue = float(nControls - nControlsMin)/
	(nControlsMax - nControlsMin);
    nControlsSlider->setValue(sliderValue);
}

void DlgNewCourse::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 300;
    int height = 300;
    int widgetXPos = 190;
    int widgetYPos = height-70;
    int widgetYPosDelta = 30;
    int widgetHeight = 20;
    int widgetWidth = 100;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Create a new course");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    int widgetY = widgetYPos - 0*widgetYPosDelta;
    name = new puInput(widgetXPos, widgetY, 
		       widgetXPos+widgetWidth, widgetY+widgetHeight);
    name->setLabel("Name:");
    name->setLabelPlace(PUPLACE_CENTERED_LEFT);

    nControlsSlider = new puSlider(widgetXPos, widgetYPos - 1*widgetYPosDelta, 
				    widgetWidth, false, widgetHeight);
    nControlsSlider->setLabel("Number of controls:");
    nControlsSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    nControlsSlider->setCallback(nControls_cb);
    nControlsSlider->setUserData(this);
    nControlsSlider->setLegend(nControlsLegend);

    ok_button = new puOneShot(10, 10, "OK");
    ok_button->setSize(buttonWidth, buttonHeight);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);

    cancel_button = new puOneShot(100, 10, "Cancel");
    cancel_button->setSize(buttonWidth, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    close();

    setDefaultValues();
    nControls_cb(nControlsSlider);
}

DlgNewCourse::~DlgNewCourse()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(nControlsSlider);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}

void DlgCourse::reshape(int x, int y, int width, int height)
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
    list_box->setSize(int(width-30-padding/2), int(height - 40 - 20 - padding));

    new_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    delete_button->setPosition(int(width/2 + 0.5*padding), padding);
}

void DlgCourse::new_ok_cb(puObject *)
{
    DlgNewCourse* dlg = (DlgNewCourse*)DlgStack::instance().top();
	Database& db = Database::instance();
    
    std::string name = dlg->getName();
    unsigned int sz = name.size();
    std::string::size_type i = name.find_first_of(' ');
    if (sz == 0)
    {	
	DlgInformation::info(dlg, "You must give name for the course");
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
		int nControls = dlg->getNumberOfControls();
		Course* course = new Course(name, terrain->id(), 
					    terrain->width(), terrain->height(), nControls);
		db.addCourse(course);
		db.updateCoursesView();
		db.setSelectedCourse(course);
		db.updateCourseObservers(0);
		dlg = (DlgNewCourse*)DlgStack::instance().pop();
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

void DlgCourse::new_cancel_cb(puObject *)
{
    puGroup* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
}

void DlgCourse::new_cb(puObject* new_button)
{
    DlgCourse* dlgCourse = (DlgCourse*)new_button->getUserData();
    DlgBase* dlgNewCourse = new DlgNewCourse(dlgCourse, 100, 100, new_ok_cb, new_cancel_cb);

    int x, y, width, height;
    DlgBase* parent = dlgCourse->getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgNewCourse->reshape(x, y, width, height);

    DlgStack::instance().push(dlgNewCourse);
}

void DlgCourse::delete_ok_cb(puObject *)
{
    try
    {
	Database& db = Database::instance();
	db.removeCourse(db.getSelectedCourse());
	db.updateCoursesView();
	db.adjustSelectedCourse();
	db.updateCourseObservers(0);
	
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

void DlgCourse::delete_cb(puObject *delete_button)
{
    DlgCourse* dlgCourse = (DlgCourse*)delete_button->getUserData();
    DlgConfirm::confirm(dlgCourse->getParent(), "Really delete?", 
			"Delete", delete_ok_cb, 
			"Cancel", &DlgBase::pop_cb);
}

void DlgCourse::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val ;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgCourse::handle_select(puObject* l_box)
{
    int selectedIndex = l_box->getIntegerValue();
    Database& db = Database::instance();
    db.setSelectedCourseIndex(selectedIndex);
    db.updateCourseObservers(0);
}

void DlgCourse::init(int width, int height, char** items)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, items);
    list_box->setLabel("Courses");
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

DlgCourse::~DlgCourse()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    puDeleteObject(new_button);
    puDeleteObject(delete_button);
}

int DlgCourse::updateCourses(void* callData) const
{
    Database& db = Database::instance();
    char** mapList = db.getSelectedCourses();
    const int selectedIndex = db.getSelectedCourseIndex();
    
    list_box->newList(mapList);
    list_box->setValue(selectedIndex);

    if (selectedIndex == -1)
	delete_button->greyOut();
    else
	delete_button->activate();
    return 0;
}

int DlgCourse::updateTerrains(void* callData) const
{
    Database& db = Database::instance();
    const int selectedIndex = db.getSelectedTerrainIndex();
    
    if (selectedIndex == -1)
	new_button->greyOut();
    else
	new_button->activate();
    return 0;
}

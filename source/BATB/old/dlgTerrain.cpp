#include <iostream>

#include <plib/pu.h>

#ifdef WIN32
#  include <TIME.H>
#endif

#include "database.h"
#include "terrain.h"

#include "dlgCommon.h"
#include "dlgTerrain.h"
#include "dlgStack.h"


class DlgNewTerrain : public DlgBase
{
public:
    std::string getName() const { return name->getStringValue(); }
    int getDimensions() const { return dimensions; }
    int getAltitudeVariation() const { return altitudeVariation; }
    int getNonFractalIterations() const { return  nonFractalIterations; }
    int getRecursionLevels() const { return recursionLevels; }
    float getFractalDimension() const { return fractalDimension ; }

    void reshape(int x, int y, int width, int height);

    DlgNewTerrain(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb, cancel_cb);
    }

    ~DlgNewTerrain();
    void print() { std::cout << "DlgNewTerrain\n"; }

private:
    puInput*  name;

    puSlider* dimensionsSlider;
    char dimensionsLegend[32];
    int dimensions;
    static int dimensionsMin;
    static int dimensionsMax;
    static int dimensionsDefault;

    puSlider* altitudeVariationSlider;
    char altitudeVariationLegend[32];
    int altitudeVariation;
    static int altitudeVariationMin;
    static int altitudeVariationMax;
    static int altitudeVariationDefault;

    puSlider* nonFractalIterationsSlider;
    char nonFractalIterationsLegend[32];
    int nonFractalIterations;
    static int nonFractalIterationsMin;
    static int nonFractalIterationsMax;
    static int nonFractalIterationsDefault;

    puSlider* recursionLevelsSlider;
    char recursionLevelsLegend[32];
    int recursionLevels;
    static int recursionLevelsMin;
    static int recursionLevelsMax;
    static int recursionLevelsDefault;

    puSlider* fractalDimensionSlider;
    char fractalDimensionLegend[32];
    float fractalDimension;
    static float fractalDimensionMin;
    static float fractalDimensionMax;
    static float fractalDimensionDefault;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);

    static void dimensions_cb(puObject* slider);
    static void altitudeVariation_cb(puObject* slider);
    static void nonFractalIterations_cb(puObject* slider);
    static void recursionLevels_cb(puObject* slider);
    static void fractalDimension_cb(puObject* slider);
};

int DlgNewTerrain::dimensionsMin = 200;
int DlgNewTerrain::dimensionsMax = 800;
int DlgNewTerrain::dimensionsDefault = 400;

int DlgNewTerrain::altitudeVariationMin = 20;
int DlgNewTerrain::altitudeVariationMax = 60;
int DlgNewTerrain::altitudeVariationDefault = 40;

int DlgNewTerrain::nonFractalIterationsMin = 1;
int DlgNewTerrain::nonFractalIterationsMax = 10;
int DlgNewTerrain::nonFractalIterationsDefault = 5;

int DlgNewTerrain::recursionLevelsMin = 2;
int DlgNewTerrain::recursionLevelsMax = 15;
int DlgNewTerrain::recursionLevelsDefault = 10;

float DlgNewTerrain::fractalDimensionMin = 0.5;
float DlgNewTerrain::fractalDimensionMax = 1.0;
float DlgNewTerrain::fractalDimensionDefault = 0.65;


void DlgNewTerrain::reshape(int x, int y, int width, int height)
{
    int widgetHeight = 20;

    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(int(width/2), int(height/2 + 2*(widgetHeight + padding)));
    name->setSize(int(width/2-2*padding), widgetHeight);

    dimensionsSlider->setPosition(int(width/2), int(height/2 + 1*(widgetHeight + padding)));
    dimensionsSlider->setSize(int(width/2-2*padding), widgetHeight);

    altitudeVariationSlider->setPosition(int(width/2), int(height/2 + 0*(widgetHeight + padding)));
    altitudeVariationSlider->setSize(int(width/2-2*padding), widgetHeight);

    nonFractalIterationsSlider->setPosition(int(width/2), int(height/2 + -1*(widgetHeight + padding)));
    nonFractalIterationsSlider->setSize(int(width/2-2*padding), widgetHeight);

    recursionLevelsSlider->setPosition(int(width/2), int(height/2 + -2*(widgetHeight + padding)));
    recursionLevelsSlider->setSize(int(width/2-2*padding), widgetHeight);

    fractalDimensionSlider->setPosition(int(width/2), int(height/2 + -3*(widgetHeight + padding)));
    fractalDimensionSlider->setSize(int(width/2-2*padding), widgetHeight);

    ok_button->setPosition(int(width/2 - 0.5*padding - buttonWidth), padding);
    cancel_button->setPosition(int(width/2 + 0.5*padding), padding);
}

void DlgNewTerrain::dimensions_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewTerrain* dlgNewTerrain = (DlgNewTerrain*)slider->getUserData();
    dlgNewTerrain->dimensions = 
	int(value*(dimensionsMax - dimensionsMin) +
	    dimensionsMin);
    sprintf(dlgNewTerrain->dimensionsLegend, "%dx%d", 
	    dlgNewTerrain->dimensions, dlgNewTerrain->dimensions);
}

void DlgNewTerrain::altitudeVariation_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewTerrain* dlgNewTerrain = (DlgNewTerrain*)slider->getUserData();
    dlgNewTerrain->altitudeVariation = 
	int(value*(altitudeVariationMax - altitudeVariationMin) + 
	    altitudeVariationMin);
    sprintf(dlgNewTerrain->altitudeVariationLegend, "%d", 
	    dlgNewTerrain->altitudeVariation);
}

void DlgNewTerrain::nonFractalIterations_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewTerrain* dlgNewTerrain = (DlgNewTerrain*)slider->getUserData();
    dlgNewTerrain->nonFractalIterations = 
	int(value*(nonFractalIterationsMax - nonFractalIterationsMin) +
	    nonFractalIterationsMin);
    sprintf(dlgNewTerrain->nonFractalIterationsLegend, "%d", 
	    dlgNewTerrain->nonFractalIterations);
}

void DlgNewTerrain::recursionLevels_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewTerrain* dlgNewTerrain = (DlgNewTerrain*)slider->getUserData();
    dlgNewTerrain->recursionLevels = 
	int(value*(recursionLevelsMax - recursionLevelsMin) +
	    recursionLevelsMin);
    sprintf(dlgNewTerrain->recursionLevelsLegend, "%d", 
	    dlgNewTerrain->recursionLevels);
}

void DlgNewTerrain::fractalDimension_cb(puObject* slider)
{
    float value = slider->getFloatValue();
    DlgNewTerrain* dlgNewTerrain = (DlgNewTerrain*)slider->getUserData();
    dlgNewTerrain->fractalDimension = 
	value*(fractalDimensionMax - fractalDimensionMin) +
	fractalDimensionMin;
    sprintf(dlgNewTerrain->fractalDimensionLegend, "%.2f", 
	    dlgNewTerrain->fractalDimension);
}

void DlgNewTerrain::setDefaultValues()
{
    dimensions = dimensionsDefault;
    altitudeVariation = altitudeVariationDefault;
    nonFractalIterations = nonFractalIterationsDefault;
    recursionLevels = recursionLevelsDefault;
    fractalDimension = fractalDimensionDefault;

    float dim = float(dimensions - dimensionsMin)/
	(dimensionsMax - dimensionsMin);
    dimensionsSlider->setValue(dim);

    float av = float(altitudeVariation - altitudeVariationMin)/
	(altitudeVariationMax - altitudeVariationMin);
    altitudeVariationSlider->setValue(av);

    float nfi = float(nonFractalIterations - nonFractalIterationsMin)/
	(nonFractalIterationsMax - nonFractalIterationsMin);
    nonFractalIterationsSlider->setValue(nfi);

    float rl = float(recursionLevels - recursionLevelsMin)/
	(recursionLevelsMax - recursionLevelsMin);
    recursionLevelsSlider->setValue(rl);

    float fd = float(fractalDimension - fractalDimensionMin)/
	(fractalDimensionMax - fractalDimensionMin);
    fractalDimensionSlider->setValue(fd);

}

void DlgNewTerrain::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 300;
    int height = 300;
    int widgetXPos = 190;
    int widgetYPos = height-70;
    int widgetYPosDelta = 30;
    int widgetHeight = 20;
    int widgetWidth = 100;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Create a new terrain");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    int widgetY = widgetYPos - 0*widgetYPosDelta;
    name = new puInput(widgetXPos, widgetY, 
		       widgetXPos+widgetWidth, widgetY+widgetHeight);
    name->setLabel("Name:");
    name->setLabelPlace(PUPLACE_CENTERED_LEFT);

    dimensionsSlider = new puSlider(widgetXPos, widgetYPos - 1*widgetYPosDelta, 
				    widgetWidth, false, widgetHeight);
    dimensionsSlider->setLabel("Dimensions:");
    dimensionsSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    dimensionsSlider->setCallback(dimensions_cb);
    dimensionsSlider->setUserData(this);
    dimensionsSlider->setLegend(dimensionsLegend);

    altitudeVariationSlider = new puSlider(widgetXPos, widgetYPos - 2*widgetYPosDelta, 
					   widgetWidth, false, widgetHeight);
    altitudeVariationSlider->setLabel("Altitude variation:");
    altitudeVariationSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    altitudeVariationSlider->setCallback(altitudeVariation_cb);
    altitudeVariationSlider->setUserData(this);
    altitudeVariationSlider->setLegend(altitudeVariationLegend);

    nonFractalIterationsSlider = new puSlider(widgetXPos, widgetYPos - 3*widgetYPosDelta, 
					      widgetWidth, false, widgetHeight);
    nonFractalIterationsSlider->setLabel("Nonfractal iterations:");
    nonFractalIterationsSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    nonFractalIterationsSlider->setCallback(nonFractalIterations_cb);
    nonFractalIterationsSlider->setUserData(this);
    nonFractalIterationsSlider->setLegend(nonFractalIterationsLegend);

    recursionLevelsSlider = new puSlider(widgetXPos, widgetYPos - 4*widgetYPosDelta, 
					 widgetWidth, false, widgetHeight);
    recursionLevelsSlider->setLabel("Recursion levels:");
    recursionLevelsSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    recursionLevelsSlider->setCallback(recursionLevels_cb);
    recursionLevelsSlider->setUserData(this);
    recursionLevelsSlider->setLegend(recursionLevelsLegend);

    fractalDimensionSlider = new puSlider(widgetXPos, widgetYPos - 5*widgetYPosDelta, 
					  widgetWidth, false, widgetHeight);
    fractalDimensionSlider->setLabel("Fractal dimension:");
    fractalDimensionSlider->setLabelPlace(PUPLACE_CENTERED_LEFT);
    fractalDimensionSlider->setCallback(fractalDimension_cb);
    fractalDimensionSlider->setUserData(this);
    fractalDimensionSlider->setLegend(fractalDimensionLegend);

    ok_button = new puOneShot(10, 10, "OK");
    ok_button->setSize(80, 30);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);

    cancel_button = new puOneShot(100, 10, "Cancel");
    cancel_button->setSize(80, 30);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    close();

    setDefaultValues();
    dimensions_cb(dimensionsSlider);
    altitudeVariation_cb(altitudeVariationSlider);
    nonFractalIterations_cb(nonFractalIterationsSlider);
    recursionLevels_cb(recursionLevelsSlider);
    fractalDimension_cb(fractalDimensionSlider);
}

DlgNewTerrain::~DlgNewTerrain()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(dimensionsSlider);
    puDeleteObject(altitudeVariationSlider);
    puDeleteObject(nonFractalIterationsSlider);
    puDeleteObject(recursionLevelsSlider);
    puDeleteObject(fractalDimensionSlider);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}

void DlgTerrain::reshape(int x, int y, int width, int height)
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

void DlgTerrain::new_ok_cb(puObject *)
{
    DlgNewTerrain* dlg = (DlgNewTerrain*)DlgStack::instance().top();
    Database& db = Database::instance();
    
    std::string name = dlg->getName();
    unsigned int sz = name.size();
    std::string::size_type i = name.find_first_of(' ');
    if (sz == 0)
    {
	DlgInformation::info(dlg, "You must give name for the terrain");
    }
    else if (0 <= i && i < sz)
    {
	DlgInformation::info(dlg, "Name may not contain space characters");
    }
    else
    {
	int dim = dlg->getDimensions();
	int av = dlg->getAltitudeVariation();
	int nfi = dlg->getNonFractalIterations();
	int rl = dlg->getRecursionLevels();
	float fd = dlg->getFractalDimension();
	int seed = time((time_t*)0);
	seed %= 900000000;

	Terrain* terrain = new Terrain(name, dim, dim, seed, av, nfi, rl, fd);
	try
	{
	    db.addTerrain(terrain);
	    db.updateTerrainsView();
	    db.setSelectedTerrain(terrain);
	    db.updateTerrainObservers(0);
	    dlg = (DlgNewTerrain*)DlgStack::instance().pop();
	    puDeleteObject(dlg);
	} 
	catch (const Exists& ex)
	{
	    delete terrain;
	    DlgInformation::info(dlg, ex.getMessage());
	}
    }
}

void DlgTerrain::new_cb(puObject* new_button)
{
    DlgTerrain* dlgTerrain = (DlgTerrain*)new_button->getUserData();
    DlgBase* dlgNewTerrain = new DlgNewTerrain(dlgTerrain, 100, 100, 
					       new_ok_cb, &DlgBase::pop_cb);
    int x, y, width, height;
    DlgBase* parent = dlgTerrain->getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgNewTerrain->reshape(x, y, width, height);
    DlgStack::instance().push(dlgNewTerrain);
}

void DlgTerrain::delete_ok_cb(puObject *)
{
    try
    {
	Database& db = Database::instance();
	db.removeTerrain(db.getSelectedTerrain());
	db.updateTerrainsView();
	db.adjustSelectedTerrain();
	db.updateTerrainObservers(0);

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

void DlgTerrain::delete_cb(puObject *delete_button)
{
    DlgTerrain* dlgTerrain = (DlgTerrain*)delete_button->getUserData();
    DlgConfirm::confirm(dlgTerrain->getParent(), "Really delete?", 
			"Delete", delete_ok_cb, 
			"Cancel", &DlgBase::pop_cb);
}

void DlgTerrain::handle_slider(puObject* slider)
{
    float val;
    slider->getValue(&val);
    val = 1.0f - val ;

    puListBox* list_box = (puListBox*)slider->getUserData();
    int idx = int(list_box->getNumItems() * val);
    list_box->setTopItem(idx);
}

void DlgTerrain::handle_select(puObject* l_box)
{
    int selectedIndex = l_box->getIntegerValue();
    Database& db = Database::instance();
    db.setSelectedTerrainIndex(selectedIndex);
    db.updateTerrainObservers(0);
}

void DlgTerrain::init(int width, int height, char** items)
{
    frame = new puFrame(0, 0, width, height);

    slider = new puSlider(width-30, 40, height-60, TRUE, 20);
    slider->setValue(1.0f);
    slider->setSliderFraction(0.2f) ;
    slider->setCBMode(PUSLIDER_DELTA);
  
    list_box = new puListBox(10, 40, width-40, height-20, items);
    list_box->setLabel("Terrains");
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

DlgTerrain::~DlgTerrain()
{
    puDeleteObject(frame);
    puDeleteObject(slider);
    puDeleteObject(list_box);
    puDeleteObject(new_button);
    puDeleteObject(delete_button);
}

int DlgTerrain::updateTerrains(void* callData) const
{
    Database& db = Database::instance();
    char** terrainList = db.getSelectedTerrains();
    const int selectedIndex = db.getSelectedTerrainIndex();
    
    list_box->newList(terrainList);
    list_box->setValue(selectedIndex);

    if (selectedIndex == -1)
	delete_button->greyOut();
    else
	delete_button->activate();
    return 0;
}

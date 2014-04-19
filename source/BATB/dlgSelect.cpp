#include <plib/pu.h>

#include "database.h"

#include "dlgCommon.h"
#include "dlgOrienteer.h"
#include "dlgSelect.h"
#include "dlgStack.h"


void DlgSelect::reshape(int x, int y, int width, int height)
{
    //const int padding = 10;

    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    dlgOrienteer->reshape(int(2*padding), int(50+padding/2),
			  int(width-4*padding), int(height-50-30-padding));

    ok_button->setPosition(width/2 - buttonWidth/2, 10);
}

DlgSelect::~DlgSelect()
{
    Database& db = Database::instance();
    db.removeObserver(dlgOrienteer);
    puDeleteObject(dlgOrienteer);

    puDeleteObject(frame);
    puDeleteObject(ok_button);
}

void DlgSelect::init(puCallback ok_cb)
{
    int width = 600;
    int height = 400;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Select orienteer");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    Database& db = Database::instance();

    dlgOrienteer = new DlgOrienteer(this, 10, 50, 250, 300);
    db.addObserver(dlgOrienteer);
    db.updateOrienteerObservers(0);

    ok_button = new puOneShot(10, 10, "OK");
    ok_button->setSize(buttonWidth, buttonHeight);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);

    resize();

    close();
}

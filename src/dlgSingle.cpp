#include <plib/pu.h>

#include "database.h"

#include "dlgCommon.h"
#include "dlgEvent.h"
#include "dlgResult.h"
#include "dlgSingle.h"
#include "dlgStack.h"


void DlgSingle::reshape(int x, int y, int width, int height)
{
    const int padding = 10;
    int bw = int(buttonWidth*1.5);

    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    dlgEvent->reshape(int(2*padding), int(50+padding/2),
		      int(width/2-2.5*padding), int(height-50-30-padding));

    dlgResult->reshape(int(width/2+0.5*padding), int(50+0.5*padding),
		       int(width/2-2.5*padding), int(height-50-30-padding));

    start_button->setPosition(width/2-5-bw, 10);
    cancel_button->setPosition(width/2+5, 10);
}

DlgSingle::~DlgSingle()
{
    Database& db = Database::instance();
    db.removeObserver(dlgEvent);
    db.removeObserver(dlgResult);

    puDeleteObject(frame);
    puDeleteObject(dlgEvent);
    puDeleteObject(dlgResult);
    puDeleteObject(start_button);
    puDeleteObject(cancel_button);
}

void DlgSingle::init(puCallback ok_cb, puCallback cancel_cb)
{
    int bw = int(buttonWidth*1.5);
    int width = 600;
    int height = 400;

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Start a singleplayer game");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    Database& db = Database::instance();

    dlgEvent = new DlgEvent(this, 10, 50, 250, 300);
    db.addObserver(dlgEvent);
    db.updateEventObservers(0);

    dlgResult = new DlgResult(this, 260, 180, 500, 300);
    db.addObserver(dlgResult);
    db.updateRunObservers(0);

    start_button = new puOneShot(10, 10, "View routes");
    start_button->setSize(bw, buttonHeight);
    start_button->setUserData(this);
    start_button->setCallback(ok_cb);

    cancel_button = new puOneShot(100, 10, "Cancel");
    cancel_button->setSize(bw, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    resize();

    close();
}


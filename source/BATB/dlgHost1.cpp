#include <plib/pu.h>

#include "dlgCommon.h"
#include "dlgHost1.h"



void DlgHost1::init(puCallback ok_cb, puCallback cancel_cb)
{
    int width = 600;
    int height = 400;
    int widgetXPos = 190;
    int widgetYPos = height-70;
    int widgetYPosDelta = 30;
    int widgetHeight = 20;
    int widgetWidth = 100;
    int bw = int(buttonWidth*1.5);

    frame = new puFrame(0, 0, width, height);
    frame->setLegend("Port number to listen to");
    frame->setLegendPlace(PUPLACE_TOP_CENTERED);

    int widgetY = widgetYPos - 0*widgetYPosDelta;
    portNumber = new puInput(widgetXPos, widgetY, 
			     widgetXPos+widgetWidth, widgetY+widgetHeight);
    portNumber->setLabel("Port number:");
    portNumber->setLabelPlace(PUPLACE_CENTERED_LEFT);

    portNumber->setValue("1234");

    ok_button = new puOneShot(10, 10, "Start server");
    ok_button->setSize(bw, buttonHeight);
    ok_button->setUserData(this);
    ok_button->setCallback(ok_cb);

    cancel_button = new puOneShot(100, 10, "Cancel");
    cancel_button->setSize(bw, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(cancel_cb);

    resize();

    close();
}

void DlgHost1::reshape(int x, int y, int width, int height)
{
    int bw = int(buttonWidth*1.5);
    setPosition(x, y);

    frame->setPosition(0, 0);
    frame->setSize(width, height);

    portNumber->setPosition(width/2, height/2);
    portNumber->setSize(width/2-2*padding, 20);
    
    ok_button->setPosition(int(width/2 - 0.5*padding - bw), 10);
    cancel_button->setPosition(int(width/2 + 0.5*padding), 10);
}

DlgHost1::~DlgHost1()
{
    puDeleteObject(frame);
    puDeleteObject(portNumber);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}

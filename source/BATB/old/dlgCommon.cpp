#include <plib/pu.h>

#include "dlgCommon.h"
#include "dlgStack.h"


void DlgInformation::btn_cb(puObject*)
{
    puGroup* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
}

void DlgInformation::init(const char* msg)
{
    frame = new puFrame(0, 0, 400, 100);

    message = msg;
    name = new puText(100, 50);
    name->setLabel(message.c_str());
    //name->setLabelPlace(PUPLACE_CENTERED_LEFT);

    button = new puOneShot(10, 10, "OK");
    button->setSize(buttonWidth, buttonHeight);
    button->setCallback(btn_cb);

    close();
}

DlgInformation::~DlgInformation()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(button);
}


void DlgInformation::reshape(int x, int y, int width, int height)
{
    puFont font = name->getLabelFont();
    int nameWidth = font.getStringWidth(message.c_str());

    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(int(0.5*width - 0.5*nameWidth), int(height/2));
    button->setPosition(int(0.5*width - 0.5*buttonWidth), padding);
}

void DlgInformation::info(DlgBase* parent, const std::string& message)
{
    DlgInformation* dlgInformation = 
	new DlgInformation(parent, 100, 100, message.c_str());
    int x, y, width, height;
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgInformation->reshape(x, y, width, height);
    DlgStack::instance().push(dlgInformation);
}


void DlgConfirm::init(const char* msg, 
		      const char* btn1Label, puCallback button1_cb, 
		      const char* btn2Label, puCallback button2_cb)
{
    frame = new puFrame(0, 0, 400, 100);
    message = msg;
    name = new puText(100, 50);
    name->setLabel(message.c_str());

    ok_button = new puOneShot(10, 10, btn1Label);
    ok_button->setSize(buttonWidth, buttonHeight);
    ok_button->setUserData(this);
    ok_button->setCallback(button1_cb);

    cancel_button = new puOneShot(100, 10, btn2Label);
    cancel_button->setSize(buttonWidth, buttonHeight);
    cancel_button->setUserData(this);
    cancel_button->setCallback(button2_cb);

    close();
}

DlgConfirm::~DlgConfirm()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(ok_button);
    puDeleteObject(cancel_button);
}


void DlgConfirm::reshape(int x, int y, int width, int height)
{
    puFont font = name->getLabelFont();
    int nameWidth = font.getStringWidth(message.c_str());

    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    name->setPosition(int(0.5*width - 0.5*nameWidth), int(0.5*height));

    ok_button->setPosition(int(0.5*width - 0.5*padding - buttonWidth), padding);
    cancel_button->setPosition(int(0.5*width + 0.5*padding), padding);
}

void DlgConfirm::confirm(DlgBase* parent, const std::string& message,
			 const char* btn1Label, puCallback btn1_cb, 
			 const char* btn2Label, puCallback btn2_cb)
{
    DlgConfirm* dlgConfirm = 
	new DlgConfirm(parent, message.c_str(), 
		       btn1Label, btn1_cb, btn2Label, btn2_cb);

    int x, y, width, height;
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    dlgConfirm->reshape(x, y, width, height);
    DlgStack::instance().push(dlgConfirm);
}

void DlgProgress::reshape(int x, int y, int width, int height)
{
    int widgetHeight = 20;

    setPosition(x, y);

    // Resize the frame widget
    frame->setPosition(0, 0);
    frame->setSize(width, height);

    puFont font = name->getLabelFont();
    int nameWidth = font.getStringWidth(message.c_str());
    name->setPosition(int(0.5*width - 0.5*nameWidth), int(0.5*height+widgetHeight));

    slider->setPosition(2*padding, int(0.5*height)-widgetHeight);
    slider->setSize(int(width-4*padding), widgetHeight);
}

void DlgProgress::init(const char* msg)
{
    frame = new puFrame(0, 0, 400, 100);

    message = msg;
    name = new puText(100, 50);
    name->setLabel(message.c_str());

    slider = new puSlider(50, 50, 20, false);
    slider->setValue(float(0.0));

    int x, y, width, height;
    DlgBase* parent = getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    reshape(x, y, width, height);

    close();
}

void DlgProgress::progress(float value)
{
    slider->setSliderFraction(value);
}

void DlgProgress::setMessage(const char* msg)
{
    message = msg;
    name->setLabel(message.c_str());
}

DlgProgress::~DlgProgress()
{
    puDeleteObject(frame);
    puDeleteObject(name);
    puDeleteObject(slider);
}


void DlgBase::pop_cb(puObject *)
{
    DlgBase* dlg = DlgStack::instance().pop();
    puDeleteObject(dlg);
}

void DlgBase::resize()
{
    int x, y, width, height;
    DlgBase* parent = getParent();
    puFrame* frame = parent->getFrame();
    parent->getPosition(&x, &y);
    frame->getSize(&width, &height);
    reshape(x, y, width, height);
}

void DlgBase::setRaceOn(bool _raceOn)
{
    raceOn = _raceOn;
}

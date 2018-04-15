#ifndef _DLGCOMMON_H
#define _DLGCOMMON_H

#include <string>
#include <iostream>
#include <plib/pu.h>


class DlgBase : public puGroup
{
 public:
    DlgBase(DlgBase* parent, int x, int y) : puGroup(x, y)
    {
	padding = 10;
	buttonWidth = 80;
	buttonHeight = 30;
	this->parent = parent;
	frame = 0;
    }
    virtual ~DlgBase() {}
    virtual void reshape(int x, int y, int width, int height) = 0;
    DlgBase* getParent() const { return parent; }
    puFrame* getFrame() const { return frame; }
    static void toggle_visible_cb(puObject *);
    static void pop_cb(puObject *);
    virtual void setRaceOn(bool _raceOn);
    virtual void print() = 0;

 protected:
    int padding;
    int buttonWidth;
    int buttonHeight;
    bool raceOn;

    DlgBase* parent;
    puFrame* frame;
    void resize();
};

class DlgInformation : public DlgBase
{
public:
    void reshape(int x, int y, int width, int height);
    static void info(DlgBase* parent, const std::string& message);
    void print() { std::cout << "DlgInformation\n";}

protected:
    puOneShot* button;
    puText* name;
    std::string message;

    void init(const char* message);
    static void btn_cb(puObject* slider);

    DlgInformation(DlgBase* parent, int x, int y, const char* message)
	: DlgBase(parent, x, y)
    {
	init(message);
    }

    ~DlgInformation();
};

class DlgConfirm : public DlgBase
{
public:
    void reshape(int x, int y, int width, int height);

    static void confirm(DlgBase* parent, const std::string& message,
			const char* btn1Label, puCallback btn1_cb, 
			const char* btn2Label, puCallback btn2_cb);
    void print() { std::cout << "DlgConfirm\n";}

protected:
    puText* name;
    std::string message;
    puOneShot* ok_button;
    puOneShot* cancel_button;

    void init(const char* message, 
	      const char* btn1Label, puCallback btn1_cb, 
	      const char* btn2Label, puCallback btn2_cb);

    DlgConfirm(DlgBase* parent, const char* message, 
	       const char* btn1Label, puCallback btn1_cb, 
	       const char* btn2Label, puCallback btn2_cb)
	: DlgBase(parent, 100, 100)
    {
	init(message, btn1Label, btn1_cb, btn2Label, btn2_cb);
    }

    ~DlgConfirm();
};

class DlgProgress : public DlgBase
{
public:

    DlgProgress(DlgBase* parent, const char* message)
	: DlgBase(parent, 100, 100)
    {
	init(message);
    }

    ~DlgProgress();

    void reshape(int x, int y, int width, int height);

    void setMessage(const char* msg);
    void progress(float value);
    void print() { std::cout << "DlgProgress\n";}

protected:
    puText* name;
    puSlider* slider;
    std::string message;

    void init(const char* message);

};

#endif // _DLGCOMMON_H

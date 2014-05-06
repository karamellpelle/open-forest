#ifndef _WINDOWMESSAGE_H
#define _WINDOWMESSAGE_H

#include <string>
#include <vector>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <plib/pu.h>


class TimerObserver
{
 public:
    virtual ~TimerObserver() {}
    virtual void timeout(int userData) = 0;
};


class WindowMessage
{
 public:
    WindowMessage(std::string msg, GLint t)
	: message(msg), time(t)
    {}
    GLint getTime() const { return time; }
    std::string getMessage() const { return message; }

 private:
    std::string message; // the message
    GLint time;    // time this message was submitted

};


class WindowMessages
{
 public:
    WindowMessages(int winWidth, int winHeight);
    ~WindowMessages();

    enum HorizontalAlignment
    {
	HA_LEFT,
	HA_CENTER,
	HA_RIGHT
    };

    enum VerticalAlignment
    {
	VA_TOP,
	VA_CENTER,
	VA_BOTTOM
    };

    void reshape(int winWidth, int winHeight);
    void addMessage(std::string msg);
    void display();
    void hideInput();
    void showInput();
    void clearInput();
    std::string getTypedText() { return textInput->getStringValue(); }

    void startTimer(int useconds, TimerObserver* to, int userData);
    void cancelTimer();

    void setMessage(HorizontalAlignment h, VerticalAlignment v, std::string msg);

 private:
    int winWidth;
    int winHeight;

    std::vector<WindowMessage> messages;
    puText* winMessages[5];

    std::string message;
    puText* winMessage;
    HorizontalAlignment horizontal;
    VerticalAlignment vertical;
    void setMessagePosition();

    puInput* textInput;
    static const int expireTime;

    bool timerActive;
    int timeoutTime;
    int userData;
    TimerObserver* timerObserver;
};

#endif // _WINDOWMESSAGE_H

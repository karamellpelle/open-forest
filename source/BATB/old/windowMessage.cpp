#include <stdexcept>
#include "windowMessage.h"

const int WindowMessages::expireTime = 5000;

WindowMessages::WindowMessages(int ww, int wh)
    : winWidth(ww), winHeight(wh), timerActive(false)
{
    puFont helvetica18(PUFONT_HELVETICA_18);
    puFont timesRoman24(PUFONT_TIMES_ROMAN_24);

#if 0
    fntTexFont sorority("data/sorority.txf");
    //puFont font(fntTexFont *tex_font, float pointsize, float slant = 0);
    puFont sorority20(&sorority, 20);
#endif

    for (int i = 0; i < 5; i++)
    {
        winMessages[i] = new puText(10, winHeight - i*15 - 25);
        winMessages[i]->setLabelFont(helvetica18);
    }

    textInput = new puInput(10, 10, winWidth-20, 20);
    textInput->hide();

    winMessage = new puText(10, 10);
    winMessage->setLabelFont(timesRoman24);

}

WindowMessages::~WindowMessages()
{
    for (int i = 0; i < 5; i++)
        puDeleteObject(winMessages[i]);
    puDeleteObject(textInput);
    puDeleteObject(winMessage);
}

void WindowMessages::hideInput() 
{
    textInput->hide(); 
    textInput->rejectInput(); 
}

void WindowMessages::showInput() 
{
    textInput->reveal(); 
    textInput->acceptInput(); 
}

void WindowMessages::clearInput() 
{
    textInput->setValue(""); 
}

void WindowMessages::reshape(int ww, int wh)
{
    winWidth = ww;
    winHeight = wh;

    for (int i = 0; i < 5; i++)
        winMessages[i]->setPosition(10, winHeight - i*15 - 25);

    textInput->setPosition(10, 10);
    textInput->setSize(winWidth-20, 20);

    setMessagePosition();
}

void WindowMessages::addMessage(std::string msg)
{
    WindowMessage wm(msg, glutGet(GLUT_ELAPSED_TIME));
    while (messages.size() >= 5)
        messages.erase(messages.begin());
    messages.push_back(wm);
}

void WindowMessages::display()
{
    GLint timeNow = glutGet(GLUT_ELAPSED_TIME);
    
    // delete old messages, looks only at the first message
    std::vector<WindowMessage>::iterator iter = messages.begin();
    if (iter != messages.end())
    {
        WindowMessage& wm = *iter;
        GLint timeThen = wm.getTime();
        if (timeNow - timeThen > expireTime) 
        {
            // message has expired, delete it
            messages.erase(iter);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        try
        {
            WindowMessage wm = messages.at(i);
            std::string msg = wm.getMessage();
            const char* message = msg.c_str();
            winMessages[i]->setLabel(message);
        } 
        catch (std::out_of_range& ex)
        {
            winMessages[i]->setLabel("");
        }
    }

    if (timerActive && (timeNow > timeoutTime))
    {
        timerActive = false;
        timerObserver->timeout(userData);
    }
}

void WindowMessages::startTimer(int useconds, TimerObserver* to, int ud)
{
    GLint timeNow = glutGet(GLUT_ELAPSED_TIME);
    timeoutTime = timeNow + useconds;
    timerActive = true;
    userData = ud;
    timerObserver = to;
}

void WindowMessages::cancelTimer()
{
    timerActive = false;
}

void WindowMessages::setMessagePosition()
{
    const char* m = message.c_str();
    puFont font = winMessage->getLabelFont();
    int msgWidth = font.getStringWidth(m);
    int msgHeight = font.getStringHeight(m);

    int x = 10;
    switch (horizontal)
    {
    case HA_LEFT:   x = 10;                           break;
    case HA_CENTER: x = int((winWidth - msgWidth)/2); break;
    case HA_RIGHT:  x = winWidth - msgWidth - 10;     break;
    }

    int y = 10;
    switch (vertical)
    {
    case VA_TOP:    y = winHeight - msgHeight - 10;     break;
    case VA_CENTER: y = int((winHeight - msgHeight)/2); break;
    case VA_BOTTOM: y = 10;                             break;
    }

    winMessage->setPosition(x, y);
}

void WindowMessages::setMessage(HorizontalAlignment h, VerticalAlignment v, std::string msg)
{
    message = msg;
    winMessage->setLabel(message.c_str());
    horizontal = h;
    vertical = v;

    setMessagePosition();
}

/*
  puFont::puFont ( void *glut_font ) ;
  puFont::puFont () ;

  extern puFont PUFONT_8_BY_13 ;        - 8x13 Fixed width
  extern puFont PUFONT_9_BY_15 ;        - 9x15 Fixed width
  extern puFont PUFONT_TIMES_ROMAN_10 ; - 10-point Proportional
  extern puFont PUFONT_TIMES_ROMAN_24 ; - 24-point Proportional
  extern puFont PUFONT_HELVETICA_10 ;   - 10-point Proportional
  extern puFont PUFONT_HELVETICA_12 ;   - 12-point Proportional
  extern puFont PUFONT_HELVETICA_18 ;   - 18-point Proportional

*/

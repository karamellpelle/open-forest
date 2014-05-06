#ifndef _ORIPROXY_H
#define _ORIPROXY_H

class OpObserver
{
 public:
    virtual ~OpObserver() {}
    virtual void punch(int controlNumber) = 0;
};

class OrienteerProxy 
{
 public:

    // how high above the ground our viewpoint is
    static const fptype orienteerHeight;

    // maximum distance from orienteer to control for the control to be visible
    static const int controlViewTreshold = 10;

    // maximum distance from orienteer to control for stamping
    static const int controlStampTreshold = 1;

    void setEventProxy(EventProxy* ep) {eventProxy = ep; }

    // implements the singleton pattern
    static OrienteerProxy& instance()
    {
	static OrienteerProxy theSingleInstance;
	return theSingleInstance;
    }
    ~OrienteerProxy();

    void reset(int id);
    void start();
    void quit();
    bool punch();
    void finish();

    Orienteer* getOrienteer();
    void setConditions(Conditions);
    void setFocus();
    void adjustStepLength(bool standingStill);
    void adjustHorizontal(bool lookingAround);
    void resetRunDirection();

    void newPosition();
    void setPosition(fptype, fptype);
    Point getPosition();
    void disqualify();
    bool runInProgress();
    void drawToMap(bool drawOthers);
    void drawRunToMap();

    Run::RunStatus getStatus() const;

    void setOpObserver(OpObserver* opObserver);

    void addVertical(fptype toAdd);
    void addDirection(fptype toAdd, bool standingStill, bool lookingAround);
    fptype getRunDirection();
    fptype getVertical();
    Location* getLocation();

    void setStepLength(fptype sl);

    Control* getNextControl();
    int getNextControlNumber();

    Run* getRun();

    fptype getTotalTime() const;

    void drawCompass(int width, int height);

    /* interface to terrain window */
    void view();
    void viewDay();
	void viewNight();

    /* interface to event proxy */
    int update(void* userData, void* callData);
    void lightUpdate(OrienteerProxy, int, char);


 private:
    Location*     location;
    Control*      nextControl;	/* next control to catch */
    int           nextControlNumber;

    Orienteer*    orienteer;	/* current orienteer */

    fptype        dircos;
    fptype        dirsin;

    Run*          run;
    GLboolean     fog;		// use fog?
    GLboolean     night;        // night? -> use lamp
    GLenum        light;
    GLboolean     headlight;
    GLfloat       stepLength; // length of the step we take

    Mesh*         mesh;
    int           terrainWidth;
    int           terrainHeight;

    EventProxy* eventProxy;

    Point         prevRecordedPosition;
    Point         prevPosition;

    OrienteerProxy();
    fptype getTargetSpeed();
    OpObserver* opObserver;

	// coordinates for each orienteer
	GLfloat model[32];
	static const int maxLights = 8;
	GLfloat pos[maxLights*3]; // position (x,y,z,1)
	GLfloat hor[maxLights];   // horizontal
	GLfloat ver[maxLights];   // vertical
};

#endif // _ORIPROXY_H

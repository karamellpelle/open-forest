#ifndef _RUN_H
#define _RUN_H

#include <string>
#include <vector>

#include "control.h"
#include "point.h"

struct SplitTime
{
    fptype time;         // time from start to this control
    fptype courseLength; // direct length from start to this cntrl
    fptype routeLength;  // length of route from start to this cntrl

    SplitTime(fptype _time, fptype _courseLength, fptype _routeLength) 
	: time(_time), courseLength(_courseLength), routeLength(_routeLength)
    {}

    ~SplitTime() {}

    std::ostream& toXML(std::ostream& ostr, int indent) const;
};

class Run
{
 public:
    enum RunStatus 
    {
	OK  = 0, // result ok
	DQF = 1, // disqualified
	DNF = 2  // did not finish
    };

    std::ostream& toXML(std::ostream& ostr, int indent) const;

    Run(std::string id, std::string name, std::string eventId, std::string orienteerId,
	time_t startTime, time_t stopTime, RunStatus status);

    Run(std::string name, std::string eventId, std::string orienteerId);

    ~Run();

    std::string id() const {return _id; }
    std::string name() const { return _name; }
    std::string eventId() const { return _eventId; }
    std::string orienteerId() const { return _orienteerId; }

    // draw the run
    void draw(bool highlight);

    // return the length of the run
    fptype trackLength();
 
    void recordPosition(const Point* const point);
    int numTimes();
    time_t stopTime();
    time_t startTime();
    void start();
    void finish();
    void punch(Control* prevControl, Control* thisControl, fptype length);
    void recordSplitTime(SplitTime* st);
    RunStatus getStatus() const;
    fptype getTotalTime() const;
    void setStatus(RunStatus);
    void setTimes(time_t startTime, time_t stopTime);
    std::string toString(int position) const;
    std::string statusAsString() const;

 private:
    std::string  _name;        // name of this run
    std::string  _eventId;     // foreign key of event
    std::string  _orienteerId; // foreign key of orienteer
    std::string  _id;          // id of this run

    time_t       _startTime;   // time when this run was started
    time_t       _stopTime;	   // time when this run was finished
    fptype       _totalDirect; // total direct distance
    fptype       _totalTime;   // total running time
    std::vector<Point*>     route;
    std::vector<SplitTime*> splitTimes;
    RunStatus _status;	   // status of this run: ok, dnf, dqf

    std::string genId();
    bool fileExists(std::string filename) const;
    void writeRoute() const;
    void writeRoute(std::ostream& ostr, int indent) const;
};


bool operator<(const Run& run1, const Run& run2);

// less than through pointer
bool run_lt(const Run* run1, const Run* run2);


#endif // _RUN_H

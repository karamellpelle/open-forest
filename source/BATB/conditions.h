#ifndef _CONDITIONS_H
#define _CONDITIONS_H

class Conditions
{
 private:
    bool _night;
    bool _fog;
    int _light;
 public:
    Conditions(bool night, bool fog, int light)
	: _night(night), _fog(fog), _light(light) {}

    bool getNight() { return _night; }
    bool getFog() { return _fog; }
    int getLight() { return _light; }
};

#endif // _CONDITIONS_H

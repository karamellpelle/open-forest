#ifndef _DLGPLAYERS_H
#define _DLGPLAYERS_H

#include "clientObserver.h"

class DlgPlayers
    : public DlgBase, public ClientObserver
{
 public:

    DlgPlayers(DlgBase* parent, int minx, int miny, int maxx, int maxy, bool enable_kick)
	: DlgBase(parent, minx, miny), enableKick(enable_kick)
    {
	init(maxx-minx, maxy-miny);
    }
	
    ~DlgPlayers();

    void reshape(int x, int y, int width, int height);

    int getSelectedIndex() { return selectedIndex; }
    void setSelectedIndex(int si) { selectedIndex = si; }

    int update(char** clientsList);
    void print() { std::cout << "DlgPlayers\n"; }

 private:
    puListBox* list_box;
    puSlider*  slider;
    puOneShot* kick_button;
    int selectedIndex;
    bool enableKick;

    void init(int width, int height);

    static void kick_cb(puObject *);
    static void kick_cancel_cb(puObject *);

    static void handle_slider(puObject* slider);
    static void handle_select(puObject*);

};

#endif // _DLGPLAYERS_H

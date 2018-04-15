#ifndef _DLGTERRAIN_H
#define _DLGTERRAIN_H

#include <iostream>

class DlgTerrain 
    : public DlgBase, public DatabaseObserver
{
 public:

    DlgTerrain(DlgBase* parent, int minx, int miny, int maxx, int maxy, char **items = NULL)
	: DlgBase(parent, minx, miny)
    {
	init(maxx-minx, maxy-miny, items);

    }
	
    ~DlgTerrain();

    void reshape(int x, int y, int width, int height);
    void print() { std::cout << "DlgTerrain\n"; }

 private:
    puListBox* list_box;
    puSlider*  slider;
    puOneShot* new_button;
    puOneShot* delete_button;
    puOneShot* ok_button;
    puOneShot* cancel_button;
    
    void init(int width, int height, char **items);

    static void new_cb(puObject *);
    static void new_ok_cb(puObject *);
    static void new_cancel_cb(puObject *);

    static void delete_cb(puObject *); 
    static void delete_ok_cb(puObject *);
    static void delete_cancel_cb(puObject *);

    static void handle_slider(puObject* slider);
    static void handle_select(puObject*);

    int updateTerrains(void* callData) const;
};

#endif // _DLGTERRAIN_H

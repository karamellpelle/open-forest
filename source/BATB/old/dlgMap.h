#ifndef _DLGMAP_H
#define _DLGMAP_H

#include <iostream>

class DlgMap
    : public DlgBase, public DatabaseObserver
{
 public:

    DlgMap(DlgBase* parent, int minx, int miny, int maxx, int maxy, char **items = NULL)
	: DlgBase(parent, minx, miny)
    {
	init(maxx-minx, maxy-miny, items);
    }
	
    ~DlgMap();

    void reshape(int x, int y, int width, int height);
    void print() { std::cout << "DlgMap\n"; }

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

    int updateMaps(void* callData) const;
    int updateTerrains(void* callData) const;
};


#endif // _DLGMAP_H

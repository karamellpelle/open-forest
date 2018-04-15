#ifndef _DLGORIENTEER_H
#define _DLGORIENTEER_H

#include <iostream>

class DlgOrienteer : public DlgBase, public DatabaseObserver
{
 public:

    DlgOrienteer(DlgBase* parent, int posx, int posy, int width, int height)
	: DlgBase(parent, posx, posy)
    {
	init(width, height);
    }
	
    ~DlgOrienteer();

    void reshape(int x, int y, int width, int height);
    void print() { std::cout << "DlgOrienteer\n"; }

 private:
    puListBox* list_box;
    puSlider*  slider;

    puOneShot* new_button;
    puOneShot* delete_button;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void init(int width, int height);

    static void new_cb(puObject *);
    static void new_ok_cb(puObject *);
    static void new_cancel_cb(puObject *);

    static void delete_cb(puObject *); 
    static void delete_ok_cb(puObject *);
    static void delete_cancel_cb(puObject *);

    static void handle_slider(puObject* slider);
    static void handle_select(puObject*);

    int updateOrienteers(void* callData) const;

};

#endif // _DLGORIENTEER_H

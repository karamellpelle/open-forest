#ifndef _DLGRESULT_H
#define _DLGRESULT_H

class DlgResult : public DlgBase, public DatabaseObserver
{
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

    int updateRuns(void* callData) const;

 public:

    DlgResult(DlgBase* parent, int posx, int posy, int width, int height)
	: DlgBase(parent, posx, posy)
    {
	init(width, height);
    }
	
    ~DlgResult();

    void reshape(int x, int y, int width, int height);
    void print() { std::cout << "DlgResult\n"; }
};

#endif // _DLGRESULT_H

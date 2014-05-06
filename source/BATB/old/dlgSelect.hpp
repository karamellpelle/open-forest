#ifndef _DLGSELECT_H
#define _DLGSELECT_H

class DlgSelect : public DlgBase, public DatabaseObserver
{
 private:

    DlgOrienteer* dlgOrienteer;

    puOneShot* ok_button;

    void setDefaultValues();

    void init(puCallback ok_cb);

 public:
    
    void reshape(int x, int y, int width, int height);

    DlgSelect(DlgBase* parent, int x, int y, puCallback ok_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb);
    }

    virtual ~DlgSelect();
    void print() { std::cout << "DlgSelect\n"; }
};

#endif // _DLGSELECT_H

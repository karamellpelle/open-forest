#ifndef _DLGSINGLE_H
#define _DLGSINGLE_H

class DlgSingle : public DlgBase, public DatabaseObserver
{
 private:

    DlgEvent* dlgEvent;
    DlgResult* dlgResult;

    puOneShot* start_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback start_cb, puCallback cancel_cb);

 public:
    
    void reshape(int x, int y, int width, int height);

    DlgSingle(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
        init(ok_cb, cancel_cb);
    }

    virtual ~DlgSingle();
    void print() { std::cout << "DlgSingle\n"; }
};

#endif // _DLGSINGLE_H

#ifndef _DLGSTACK_H
#define _DLGSTACK_H

#include <vector>
#include <plib/pu.h>

class DlgStack
{
 public:
    void push(DlgBase* dlg);
    DlgBase* pop();
    DlgBase* top();
    void setVisible(bool visible);
    void reshape(int x, int y, int width, int height);
    void print();

    static DlgStack& instance()
    {
	static DlgStack theSingleInstance;
	return theSingleInstance;
    }

 private:
    bool topVisible;
    std::vector<DlgBase*> dialogs;
    DlgStack() : topVisible(true) {}
    void visibility();
};

#endif //  _DLGSTACK_H

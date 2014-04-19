#include <stdexcept>

#include <plib/pu.h>

#include "util.h"
#include "dlgCommon.h"
#include "dlgStack.h"


void DlgStack:: push(DlgBase* newTop) 
{ 
#if 0
    DlgBase* oldTop = top();
    if (oldTop != NULL)
	oldTop->hide();
    newTop->reveal();
    dialogs.push_back(newTop); 
#else
    dialogs.push_back(newTop); 
    visibility();
#endif
}

DlgBase* DlgStack::pop() 
{ 
#if 0
    DlgBase* oldTop = top();
    dialogs.pop_back();
    DlgBase* newTop = top();
    oldTop->hide();
    if (newTop != NULL)
	newTop->reveal();

    return oldTop;
#else
    DlgBase* _top =  top();
    dialogs.pop_back();
    visibility();
    return _top;
#endif
}

DlgBase* DlgStack::top() 
{ 
    DlgBase* _top = NULL;
    try
    {
	_top = dialogs.at(dialogs.size()-1); 
    }
    catch (std::out_of_range&)
    {
    }
    return _top;
}

void DlgStack::visibility()
{
#if 0
    DlgBase* _top = top();
    if (_top != NULL)
    {
	if (topVisible)
	    _top->reveal();
	else
	    _top->hide();
    }
#else
    int sz = dialogs.size();
    for (int i = 0; i < sz; i++)
    {
	DlgBase* dlg = dialogs.at(i);
	if (topVisible && i == sz - 1)
	    dlg->reveal();
	else
	    dlg->hide();
    }
#endif
}

void DlgStack::setVisible(bool visible) 
{ 
#if 0
    DlgBase* _top = top();
    if (!visible) 
	_top->hide();
    else
	_top->reveal();
#else
    topVisible = visible;
    visibility();
#endif
}

void DlgStack::reshape(int x, int y, int width, int height) 
{
    std::for_each(dialogs.begin(), dialogs.end(), Reshape<DlgBase*>(x, y, width, height));
}

void DlgStack::print()
{
    std::for_each(dialogs.begin(), dialogs.end(), PrintDialog<DlgBase*>());
}


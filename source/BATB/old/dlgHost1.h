#ifndef _DLGHOST1_H
#define _DLGHOST1_H

// the first dialog in hosting a multiplayer game
// asks for a port number to listen to

class DlgHost1 : public DlgBase
{
public:
    std::string getPortNumber() { return portNumber->getStringValue(); }
    // PURPOSE:
    // REQUIRES: 
    // PROMISES: 

    void reshape(int x, int y, int width, int height);
    // PURPOSE: Reshape all the UI components in this dialog
    // REQUIRES: 
    // PROMISES: 

    DlgHost1(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb, cancel_cb);
    }

    virtual ~DlgHost1();
    void print() { std::cout << "DlgHost1\n"; }

private:
    puInput* portNumber;

    puOneShot* ok_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);
};

#endif // _DLGHOST1_H

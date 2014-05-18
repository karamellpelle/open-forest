#ifndef _DLGJOIN1_H
#define _DLGJOIN1_H

#include <plib/puAux.h>

// the first dialog in joining a multiplayer game
// asks the hostname and port number to connect to

class DlgJoin1 : public DlgBase, public DatabaseObserver
{
public:
    std::string getHostName() { return hostName->getStringValue(); }
    std::string getPortNumber() { return portNumber->getStringValue(); }
    void reshape(int x, int y, int width, int height);

    DlgJoin1(DlgBase* parent, int x, int y, puCallback ok_cb, puCallback cancel_cb)
	: DlgBase(parent, x, y)
    {
	init(ok_cb, cancel_cb);
    }

    virtual ~DlgJoin1();
    int updateServers(void* callData) const;
    void print() { std::cout << "DlgJoin1\n"; }

private:
    puaComboBox* hostName;
    //puInput* hostName;

    puInput* portNumber;

    puOneShot* connect_button;
    puOneShot* cancel_button;

    void setDefaultValues();

    void init(puCallback ok_cb, puCallback cancel_cb);
    static void hostname_cb(puObject *hostname);
};

#endif // _DLGJOIN1_H

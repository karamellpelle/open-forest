#ifndef _DLGJOIN2_H
#define _DLGJOIN2_H

#include  "client.h"
#include  "dlgPlayers.h"


class DlgJoin2 : public DlgBase, public ClientObserver
{
public:
    void reshape(int x, int y, int width, int height);
    // PURPOSE: Reshape all the UI components in this dialog
    // REQUIRES: 
    // PROMISES: 

    DlgJoin2(DlgBase* parent, int x, int y, 
	     puCallback quit_cb, puCallback disconnect_cb)
	: DlgBase(parent, x, y)
    {
	init(quit_cb, disconnect_cb);
    }

    virtual ~DlgJoin2();

    void setClient(Client* client);
    void setRaceOn(bool race_on);
    int update(char** clientsList);
    void print() { std::cout << "DlgJoin2\n"; }

private:
    DlgPlayers* dlgPlayers;

    puOneShot* quit_button;
    puOneShot* disconnect_button;

    void setDefaultValues();
    static void quit_cb(puObject *);
    static void disconnect_cb(puObject *);

    puCallback quit_cb_;
    puCallback disconnect_cb_;
    bool raceOn;

    void init(puCallback quit_cb, puCallback disconnect_cb);
};

#endif // _DLGJOIN2_H

#ifndef _DLGHOST2_H
#define _DLGHOST2_H

#include  "server.h"
#include  "dlgEvent.h"
#include  "dlgPlayers.h"

// the first dialog in hosting a multiplayer game
// asks for a port number to listen to


class DlgHost2 : public DlgBase, public ClientObserver
{
public:

    void reshape(int x, int y, int width, int height);
    // PURPOSE: Reshape all the UI components in this dialog
    // REQUIRES: 
    // PROMISES: 

    DlgHost2(DlgBase* parent, int x, int y, 
	     puCallback start_race_cb, puCallback stop_race_cb, puCallback stop_server_cb)
	: DlgBase(parent, x, y)
    {
	init(start_race_cb, stop_race_cb, stop_server_cb);
    }

    virtual ~DlgHost2();
    int getSelectedIndex() { return dlgPlayers->getSelectedIndex(); }
    void setSelectedIndex(int si) { dlgPlayers->setSelectedIndex(si); }
    void setRaceOn(bool race_on);

    // DatabaseObserver
    int updateEvents(void* callData) const;

    // ClientObserver
    int update(char** clientsList);

    void print() { std::cout << "DlgHost2\n"; }

private:
    DlgEvent* dlgEvent;
    DlgPlayers* dlgPlayers;

    puOneShot* start_race_button;
    puOneShot* stop_race_button;
    puOneShot* stop_server_button;

    void setDefaultValues();
    static void startRace_cb(puObject *);
    static void stopRace_cb(puObject *);
    static void stopServer_cb(puObject *);

    puCallback start_race_cb;
    puCallback stop_race_cb;
    puCallback stop_server_cb;

    void init(puCallback start_race_cb, puCallback stop_race_cb, puCallback stop_server_cb);
};

#endif // _DLGHOST2_H

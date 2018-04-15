#ifndef _DLGMENU_H
#define _DLGMENU_H

class DlgMenu : public DlgBase, public DatabaseObserver
{
 public:
    
    //static void MsgJoinAccept();
    void reshape(int x, int y, int width, int height);

    DlgMenu(DlgBase* parent, int x, int y)
	: DlgBase(parent, x, y)
    {
	init();
    }

    virtual ~DlgMenu();
    void print() { std::cout << "DlgMenu\n"; }

 private:
    puOneShot* select_orienteer_button;
    puOneShot* start_singleplayer_button;
    puOneShot* join_multiplayer_button;
    puOneShot* host_multiplayer_button;
    puOneShot* view_results_button;
    puOneShot* quit_button;

    static void cancel_cb(puObject *btn);
    static void single_start_cb(puObject *btn);

    static void host1_start_server_cb(puObject *dlg);
    static void host2_start_race_cb(puObject *dlg);
    static void host2_stop_race_cb(puObject *dlg);
    static void host2_stop_server_cb(puObject *dlg);

    static void join1_connect_cb(puObject *dlg);
    static void join2_quit_cb(puObject *dlg);
    static void join2_disconnect_cb(puObject *dlg);

    static void select_orienteer_cb(puObject *);
    static void start_singleplayer_cb(puObject *);
    static void join_multiplayer_cb(puObject *);
    static void host_multiplayer_cb(puObject *);
    static void view_results_cb(puObject *);
    static void quit_cb(puObject *);

    void init();
};

#endif // _DLGMENU_H

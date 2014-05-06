#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "windowMessage.h"
#include "fdManager.h"
#include "server.h"
#include "client.h"
#include "message.h"
#include "map.h"
#include "course.h"
#include "mesh.h"
#include "terrain.h"
#include "terrainproxy.h"
#include "observer.h"
#include "conditions.h"
#include "eventproxy.h"
#include "oriproxy.h"
#include "dlgCommon.h"


class MainWindow 
:   public MessageHandler, 
    public OpObserver, 
    public TimerObserver, 
    public DatabaseObserver
{
 public:

    typedef void (*continuationFunction)();
    typedef void (*DisplayFunc)();

    static DisplayFunc terrainDF;
    static DisplayFunc mapDF;

    static int main_window;
    static int winPosX;
    static int winPosY;
    static int winWidth;
    static int winHeight;
    static int oldWinWidth;
    static int oldWinHeight;
    static float framesPerSecond;

    static void initLights();
    static void init();

	// dialog callbacks
    static void save_run();
    static void save_run_ok_cb(puObject *dlg);
    static void save_run_cancel_cb(puObject *dlg);
    static void quit_run_ok_cb(puObject *);
    static void quit_run_cancel_cb(puObject *);
    static void kick_ok_cb(puObject*);
    static void host1_start_server_cb(puObject *dlg);
    static void host2_start_race_cb(puObject *dlg);
    static void host2_stop_race_cb(puObject *dlg);
    static void host2_stop_server_cb(puObject *dlg);
    static void join1_connect_cb(puObject *dlg);
    static void join2_disconnect_cb(puObject *dlg);
    static void join2_quit_cb(puObject *dlg);
    static void single_start_cb(puObject *);
    static void single_start_continuation();
    static void view_results_cb(puObject *);
    static void view_results_continuation();
    static void show_location_cb(puObject *cb);
    static void show_route_cb(puObject *cb);
    static void set_location_cb(puObject *cb);
    static void quit_ok_cb(puObject *);

    void startCountdown();
    static void startRace();
    static void stopRace();

    static void pushDisplayFuncs(void);
    static void realizeMap();
    static void realizeTerrain(continuationFunction cf);
    static void callContinuation(void);

	// glut callbacks
    static void reshapefn(int width, int height);
    static void generatedisplayfn();
    static void displayfnDay();
    static void displayMapDay(void);
    static void displayTerrainDay(void);
    static void displayTerrainNight(void);
    static void visibility(int state);
    static void mousefn(int button, int updown, int x, int y);

    static void checkSockets();
    static void actionMapping(int mouseButton);
    static void generateMapDisplayFn(void);
    static void generateTerrainDisplayFn(void);
    static void motionfn(int x, int y);
    static void keyDownFn(unsigned char key, int, int);
    static void keyUpFn(unsigned char key, int, int);
    static void specialDownFn(int key, int, int);
    static void specialUpFn(int key, int, int);
    static void displaySwitch();
    static void displayTerrain();
    static void displayMap();
    static void projection();


    static void startServer(unsigned short port);
    static Server* getServer();
    static Client* getClient();
    static void connect(std::string hostName, unsigned short portNumber);

    void serverExited();

    static void msgEventInfoContinuation();

	// message handlers
    bool handle(const MsgJoinAccept& msg, TcpServer* fromTcpServer);
    bool handle(const MsgJoinReject& msg, TcpServer* fromTcpServer);
    bool handle(const MsgEventInfo& msg, TcpServer* fromTcpServer);
    bool handle(const MsgStartRace& msg, TcpServer* fromTcpServer);
    bool handle(const MsgStopRace& msg, TcpServer* fromTcpServer);
    bool handle(const MsgNewClient& msg, TcpServer* fromTcpServer);
    bool handle(const MsgKick& msg, TcpServer* fromTcpServer);
    bool handle(const MsgDiscConf& msg, TcpServer* fromTcpServer);
    bool handle(const MsgRemoveClient& msg, TcpServer* fromTcpServer);
    bool handle(const MsgJoinReq& msgJoinReq, TcpClient* tcpClient);
    bool handle(const MsgReadyToGo& msg, TcpClient* tcpClient);
    bool handle(const MsgPunch& msg, TcpClient* sender);
    bool handle(const MsgFinished& msg, TcpClient* sender);
    bool handle(const MsgQuit& msg, TcpClient* sender);
    bool handle(const MsgCheat& msg, TcpClient* sender);
    bool handle(const MsgDiscReq& msgDiscReq, TcpClient* tcpClient);
    bool handle(const MsgChat& msg, UdpServer* fromUdpServer);
    bool handle(const MsgPosition& msg, UdpServer* fromUdpServer);
    bool handle(const MsgPunch& msg, UdpServer* fromUdpServer);
    bool handle(const MsgFinished& msg, UdpServer* fromUdpServer);
    bool handle(const MsgQuit& msg, UdpServer* fromUdpServer);
    bool handle(const MsgCheat& msg, UdpServer* fromUdpServer);

    void chat(std::string message);
    void punch(int controlNumber);

    void timeout(int userData);

    int updateEvents(void* callData) const;

    // glut display function management
    static void setDisplayFunc(DisplayFunc displayFunc);
    static void pushDisplayFunc(DisplayFunc displayFunc);
    static void popDisplayFunc();
    static void enableDisplayFunc();
    static void disableDisplayFunc();

 private:
    static void updateDisplayFunc();

    static puText* fps_text;

    static puButton* show_location_button;
    static void show_location();
    static void show_location_ok_cb(puObject *cb);
    static void show_location_cancel_cb(puObject *cb);

    static puButton* show_route_button;
    static void show_route();
    static void show_route_ok_cb(puObject *cb);
    static void show_route_cancel_cb(puObject *cb);

    static puButton* set_location_button;
    static void set_location();
    static void set_location_ok_cb(puObject *cb);
    static void set_location_cancel_cb(puObject *cb);

    static void unproject(fptype x, fptype y, fptype z,
			  fptype* objx, fptype* objy, fptype* objz);

    static void resetState(bool game_on, bool show_dialogs);

#ifdef WIN32
    static void socketStartup();
    static void socketCleanup();
#endif
    static bool showMap;
    static fptype mapScale; // current map scaling
    static fptype mapScaleMin;
    static fptype mapScaleMax;
    static fptype mapRotate; // map rotation
    static Point mapEye; // map view location 
    
    static Point thumb; // location marker on map

    static Point mouse;
    static char mouseButton;
    static bool standingStill;
    static bool lookingAround;
    static bool movingMap;
    static bool rotatingMap;
    static bool zoomingMap;
    static bool gameOn;
    static bool typingMessage;
    static bool showDialogs;
    static bool showFps;
    static bool fullScreen;
    static bool showLocation;
    static bool showRoute;
    static bool showRoutes;
    static bool settingLocation;

    static Course* course;
    static char fpsTxt[32];
    
    static WindowMessages* windowMessages;
    
    static FdManager fdManager;
    static Server* server;
    static Client* client;

    static DlgProgress* dlgProgress;
    static continuationFunction continuationFunc;

    static std::vector<DisplayFunc> displayFuncs;
    static bool displayFuncEnabled;
};

extern MainWindow mainWindow;
extern GLuint dayShader;   // shader for day
extern GLuint nightShader; // shader for night
extern GLuint nightTrees;  // shader for night


#endif // _MAINWINDOW_H

#ifndef _CLIENT_H
#define _CLIENT_H

#ifdef WIN32
#  include <windows.h>
#  include <TIME.H> 
#  include <io.h>
#else
#  include <unistd.h>
#  include <sys/time.h> 
#  include <netinet/in.h> 
#  include <sys/socket.h> 
#  include <sys/wait.h> 
#  include <arpa/inet.h>
#  include <netdb.h> 
#endif

#include <sys/types.h> 

#include "fdManager.h"
#include "message.h"
#include "observer.h"
#include "clientObserver.h"

class UdpClient;
class Client;

class UdpServer 
{
 public:
    UdpServer(Client* client, int portStart, MessageHandler* mh);
    ~UdpServer();
    int getPort() const { return port; }
    int getSocket() const { return sockfd; }
    int getId() const { return id; }
    void setId(int _id) { id = _id; }
    void send(UdpClient* receiver, std::string& msg) const;
    bool receive(std::string buf);

    std::string encodeMsgChat(std::string) const;
    std::string encodeMsgPosition(Location* location) const;
    std::string encodeMsgRun(Run* run) const;
    std::string encodeMsgPunch(int controlNumber) const;
    std::string encodeMsgFinished(fptype totalTime) const;
    std::string encodeMsgQuit() const;
    std::string encodeMsgCheat() const;

    void sendMsgPosition(UdpClient* udpClient, Location* location);

 private:
    int sockfd;
    int port; // port we will be listening to
    int id;   // my unique id
    MessageBuffer mb; // buffer for receiving messages
    Client* client;
    MessageHandler* messageHandler;

    void init(int portStart);
};

class UdpClient: public Observer
{
 public:
    enum State {
	generating,
	ready,
	started,
	quit,
	cheated,
	finished
    };

    UdpClient(int id, std::string ip, int port, std::string name);
    virtual ~UdpClient();

    void setUdpServer(UdpServer* udpServer);
    void reset();
    int  getId() const { return id; }
    std::string getIp() const { return ip; }
    int getPort() const { return port; }
    std::string getName() const { return name; }
    void setLocation(fptype x, fptype y, fptype z, fptype horizontal, fptype vertical);
    
    Point getPosition();
    fptype getRunDirection();

    std::string stateToString() const;
    std::string toString() const;
    int print();

    // begin Observer implementation
    int drawToTerrain(Location* location);
    int drawToTerrainDay(Location* location);
    int drawToTerrainNight(Location* location);
    int drawToMap(Location* location);
	void getCoordinates(GLfloat* pos, GLfloat* hor, GLfloat* ver, int index);
    // end Observer implementation

    void punch(int controlNumber);
    void setState(State newstate) { state = newstate; }

    bool operator<(const UdpClient& other);

 private:
    int         id;   // unique id of the peer
    std::string name; // name of the peer
    std::string ip;   // ip address of the peer
    int         port; // port number of the peer
    Location    location;
    UdpServer*  udpServer; // for sending messages
    State       state;
    int         punchedControl; // highest control number punched

    void drawToTerrain();
    void drawToMap();
};


class TcpServer 
{
 public:
    TcpServer(Client* client, MessageHandler* mh);
    ~TcpServer();

    int getSocket() const { return sockfd; }
    std::string getHostname() const {return hostname; }
    int getPort() const { return port; }

    bool isConnected() const { return connected; }
    bool connect(std::string address, int port);
    void disconnect();

    void send(std::string msg) const;
    void sendDiscReq();
    void sendJoinReq(std::string myName, int port);
    void sendReadyToGo();
    bool receive(std::string buf);
    std::string toString();
    void print();

 private:
    // hostname and port where the server is listening 
    std::string hostname;
    int         port;

    int sockfd; // the socket we are using
    bool connected; // are we connected to the server?

    MessageBuffer mb; // buffer used in receiving messages
    Client* client;
    MessageHandler* messageHandler;

};

class ConnectError : public std::runtime_error 
{
 public:
    ConnectError() : std::runtime_error("ConnectError") { }
};


class Client : public SocketHandler
{
 public:
    typedef std::vector<UdpClient*> UdpClients;
    typedef std::vector<ClientObserver*> ClientObservers;
    Client(int startport, FdManager& _fdManager, MessageHandler* mh);
    ~Client();

    UdpServer* getUdpServer() const { return udpServer; }
    void connect(std::string hostname, int tcpPort);
    void sendJoinReq(std::string name);
    void sendDiscReq() const { tcpServer->sendDiscReq(); }

    UdpClients getClients() { return udpClients; }
    std::string getName() const { return name; }
    void sendChat(std::string buf);
    // send chat to udp client at <index> in list
    void sendChat(int index, std::string buf);

    void sendReadyToGo() const;
    void sendPunch(int controlNumber) const;
    void sendFinished(fptype totalTime) const;
    void sendQuit() const;
    void sendCheat() const;
    void sendAll(std::string& packet) const;

    void setId(int myId) { udpServer->setId(myId); }
    int getId() { return udpServer->getId(); }

    void addUdpClient(UdpClient* udpClient);
    void disconnectTcp();
    // remove the client with id from our list of clients
    // return the removed client
    UdpClient* removeUdpClient(int id);
    void printUdpClients();

    bool handle(int sockfd);
    UdpClient* udpClientById(int udpClientId);

    void addClientObserver(ClientObserver* const clientObserver);
    void removeClientObserver(ClientObserver* const clientObserver);
    void notifyClientObservers();

    void updateClientsView();

 private:
    UdpClients udpClients;
    char** clientsList;
    int clientsListSize;
    ClientObservers clientObservers;
    FdManager&  fdManager;
    MessageHandler* messageHandler;

    std::string name;
    int         port;
    UdpServer*  udpServer; // communication with udp peers
    TcpServer*  tcpServer; // communicate with tcpServer

    bool handleMsgChat(MsgChat& message);
    bool handleMsgPosition(MsgPosition& message);
    void deleteAllUdpClients();
    bool fromTcpServer(int sockfd);
    bool fromUdpPeer(int sockfd);

    void freeClientsList();
    char** getClientsList();
};

#endif // _CLIENT_H

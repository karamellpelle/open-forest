#ifndef _SERVER_H
#define _SERVER_H

#ifdef WIN32
#  include <windows.h>
#  include <TIME.H> 
#  include <io.h>
typedef int socklen_t;
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

#include "clientObserver.h"


class Server;

/************** TCPCLIENT ************************************/

class TcpClient
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

    TcpClient(Server* server, std::string ip, int sockfd, MessageHandler* mh);
    ~TcpClient();

    void setState(State newstate) { state = newstate; }
    void disconnect() const;
    int getId() const { return id; }
    void setName(std::string name1) { name = name1; }
    std::string getName() const { return name; }
    std::string getIp() const { return ip; }

    int getSocket() const { return sockfd; }
    int getPort() const { return port; }
    void setPort(int port1) { port = port1; }

    void setReadyToGo(bool readyToGo);
    int send(std::string msg);
    void sendJoinAccept();
    void sendJoinReject();
    int sendEventInfo();
    void sendDiscConf();
    int sendKick();
    std::string encodeNewClient();
    std::string encodeRemoveClient();
    bool receive(std::string buf);
    std::string toString() const;
    int print();

    void punch(int controlNumber);

    bool operator<(const TcpClient& other);

 private:
    std::string stateToString() const;

    static int    nextId;    // id to be assigned to clients
    int           id;        // client unique id
    std::string   name;      // name of this client
    std::string   ip;        // ip address of this client
    int           sockfd;
    int           port;      // udp port number this client is listening to
    //CircularBuffer cb;     // buffer where we store partially received messages
    MessageBuffer mb;
    State         state;
    Server*       server;
    MessageHandler* messageHandler;
    int           punchedControl; // highest control number punched
};

/************** TCPLISTENER **************************************/

class TcpListenerError : public std::runtime_error 
{
 public:
    TcpListenerError() : std::runtime_error("TcpListenerError") { }
};


class TcpListener
{
 private:
    int port;   // port this listener is listening to
    int sockfd;
    int init(int port);

 public:
    TcpListener(int port);
    int getSocket() const { return sockfd; }
    int getPort() const { return port; }
};

/******************* SERVER *******************************/

class Server : public SocketHandler //, public MessageHandler
{
 public:
    typedef std::vector<TcpClient*> TcpClients;
    typedef std::vector<ClientObserver*> ClientObservers;

    Server(int port, FdManager& fdManager, MessageHandler* mh);
    ~Server();

    TcpClients& getClients();
    int getClientCount();

    // kick the tcp client located at <index> in list
    void kickClient(int index);
    void kickAllClients();

    void sendStartRaceAll();
    void sendStopRaceAll();
    void sendEventInfoAll();
    void printTcpClients();

    // Sockethandler
    bool handle(int sockfd);

    void addClientObserver(ClientObserver* const clientObserver);
    void removeClientObserver(ClientObserver* const clientObserver);
    void notifyClientObservers();

    void setExiting(bool exiting);
    bool getExiting() const;
    void removeTcpClient(TcpClient* tcpClient);
    void sendNewClientAll(TcpClient* newTcpClient);
    void sendRemoveClientAll(TcpClient* leavingTcpClient);
    void updateClientsView();

 private:
    TcpClients tcpClients;
    char** clientsList;
    int clientsListSize;
    ClientObservers clientObservers;

    TcpListener* tcpListener;
    FdManager& fdManager;
    //ObserverManager observerManager;
    int port;
    bool serverExiting;

    bool fromTcpClient(int sockfd, TcpClient* tcpClient);
    bool fromTcpListener(int sockfd);
    void sendPacketAll(std::string packet);

    void freeClientsList();
    char** getClientsList();
    MessageHandler* messageHandler;
};

#endif // _SERVER_H

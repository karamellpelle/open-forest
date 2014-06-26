#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <strings.h>

#include "database.h"
#include "server.h"
#include "util.h"

#if 1
#define MSGDEBUG
#endif

/************** TCPCLIENT ************************************/

int TcpClient::nextId = 0;

TcpClient::TcpClient(Server* _server, std::string _ip, int _sockfd, MessageHandler* mh)
    : id(nextId++), ip(_ip), sockfd(_sockfd), port(0), 
      state(generating), server(_server), messageHandler(mh), punchedControl(0)
{
}

bool TcpClient::receive(std::string buf)
{
#ifdef MSGDEBUG
    std::cout << "Read characters from client: (" << buf << ")\n";
#endif
    bool continu3;
    bool delHandler = false;
    mb.append(buf);
    try
    {
	while (true)
	{
	    std::string packet = mb.getMessage(); // throws MessageNotReady
	    std::istringstream istr(packet);
	    Message* message = Message::decode(istr);
#ifdef MSGDEBUG
	    std::cout << "TcpClient received message:\n";
	    message->print();
#endif
	    continu3 = message->handle(messageHandler, this);
	    if (!continu3)
	    {
		delHandler = true;
		break;
	    }
	}
    } 
    catch (const MessageNotReady&)
    {
    }
    return delHandler;
}

int TcpClient::send(std::string msg)
{
    if (::send(sockfd, msg.c_str(), msg.length(), 0) == -1) 
    {
	perror("send");
    }
    return 0;
}

TcpClient::~TcpClient()
{
}

std::string TcpClient::stateToString() const 
{
    switch (state)
    {
    case started:    return "Started";
    case ready:      return "Ready";
    case generating: return "Generating";
    case finished:   return "Finished";
    case quit:       return "dnf";
    case cheated:    return "dqf";
    default:         return "Unknown";
    }
}

int TcpClient::print()
{
    std::cout << name << "\n";
    return 0;
}

void TcpClient::sendJoinAccept()
{
    std::ostringstream ostr;
    MsgJoinAccept msg(id);
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());
}

void TcpClient::sendJoinReject()
{
    std::ostringstream ostr;
    MsgJoinReject msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());
}

int TcpClient::sendEventInfo()
{
    Database& db = Database::instance();
#if 0
    Terrain* terrain = db.getSelectedTerrain();
    Map* map = db.getSelectedMap();
    Course* course = db.getSelectedCourse();
#else
    Terrain* terrain = db.getEventTerrain();
    Map* map = db.getEventMap();
    Course* course = db.getEventCourse();
#endif
    Event* event = db.getSelectedEvent();
    MsgEventInfo msg(terrain, map, course, event);
    std::ostringstream ostr;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());

	return 0;
}

void TcpClient::disconnect() const 
{
#ifdef WIN32
    closesocket(getSocket()); 
#else
    close(getSocket()); 
#endif
}

void TcpClient::sendDiscConf()
{
    std::ostringstream ostr;
    MsgDiscConf msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());
}

int TcpClient::sendKick()
{
    std::ostringstream ostr;
    MsgKick msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());

    return 0;
}

// encode this into a msgNewClient, encode the message into packet
std::string TcpClient::encodeNewClient()
{
    std::ostringstream ostr;
    MsgNewClient msg(id, ip, port, name);
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    return ostr.str();
}

std::string TcpClient::encodeRemoveClient()
{
    std::ostringstream ostr;
    MsgRemoveClient msg(id);
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    return ostr.str();
}

void TcpClient::punch(int controlNumber)
{
    punchedControl = controlNumber;
}

std::string TcpClient::toString() const
{
    std::ostringstream ostr;
    std::string state = stateToString();
    ostr << id << " " 
	 << name << " "
	 << ip << " "
	 << port << " "
	 << state << " "
	 << punchedControl;
    return ostr.str();
}

bool TcpClient::operator<(const TcpClient& other)
{
    return punchedControl < other.punchedControl;
}


/************** TCPLISTENER **************************************/

#define BACKLOG 10      /* how many pending connections queue will hold */

int TcpListener::init(int port)
{
    int sockfd;
    struct sockaddr_in my_addr; // my address information
    int yes = 1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

#ifdef WIN32
    if (sockfd == INVALID_SOCKET) 
	{
	int code = WSAGetLastError();
	throw TcpListenerError();
    }
#else
    if (sockfd == -1) 
    {
	perror("socket");
	throw TcpListenerError();
    }
#endif
  
    // lose the pesky "Address already in use" error message
#ifdef WIN32
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1) 
#else
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
#endif
    {
	perror("setsockopt");
	throw TcpListenerError();
    } 

    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_port = htons(port); /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
#ifdef WIN32
    memset(&(my_addr.sin_zero), 0, 8);
#else
    bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */
#endif
  
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
    {
	perror("bind");
	throw TcpListenerError();
    }
  
    if (listen(sockfd, BACKLOG) == -1) 
    {
	perror("listen");
	throw TcpListenerError();
    }
    printf("Listening for incoming connections on port %hd\n", port);
    return sockfd;
}

TcpListener::TcpListener(int _port)
{
    try
    {
	port = _port;
	sockfd = init(port);
    }
    catch (TcpListenerError&)
    {
	throw;
    }
}

/******************* SERVER *******************************/

void Server::printTcpClients()
{
    std::cout << "Current clients are:\n";
    std::for_each(tcpClients.begin(), tcpClients.end(), std::mem_fun(&TcpClient::print));
    std::cout << "End of clients\n";
}

void Server::removeTcpClient(TcpClient* tcpClient)
{
    TcpClients::iterator i =
		std::remove(tcpClients.begin(), tcpClients.end(), tcpClient);
    if (i != tcpClients.end())
    {
        tcpClients.erase(i, tcpClients.end());
#if 0
        fdManager.remove(tcpClient->getSocket());
#endif
        std::cout << "Client " << tcpClient->getName() << " has left\n";
    }
    else
    {
        std::cout << "removeTcpClient: could not remove client " 
		  << tcpClient->getName()<< "\n";
    }
    printTcpClients();
}

void Server::sendRemoveClientAll(TcpClient* leavingTcpClient)
{
    std::string leavingPacket = leavingTcpClient->encodeRemoveClient();

    for (TcpClients::iterator i = tcpClients.begin(); 
	 i != tcpClients.end(); ++i) 
    {
        TcpClient* remainingTcpClient = *i;
        if (leavingTcpClient != remainingTcpClient) 
        {
            std::string remainingPacket = remainingTcpClient->encodeRemoveClient();
            leavingTcpClient->send(remainingPacket);
            remainingTcpClient->send(leavingPacket);
        }
    }
}

// server receives a message from tcp client
bool Server::fromTcpClient(int sockfd, TcpClient* tcpClient)
{
#define MAXDATASIZE 4000
    char buf[MAXDATASIZE];
    bool delHandler = false;

    int nbytes = recv(sockfd, buf, MAXDATASIZE, 0);
    if (nbytes < 0) 
    {
        printf("Error reading tcp socket\n");
    }
    else if (nbytes == 0) 
    {
        // client has closed the socket
        removeTcpClient(tcpClient);
        sendRemoveClientAll(tcpClient);
        tcpClient->disconnect();
        delete tcpClient;
	delHandler = true;
    }
    else 
    {
        buf[nbytes] = '\0';
        delHandler = tcpClient->receive(buf);
    }
    return delHandler;
}

// server has a new connection from a tcp client
bool Server::fromTcpListener(int sockfd)
{
    unsigned short port;
    char* addr;
    int sin_size;
    struct sockaddr_in their_addr; /* connector's address information */
    int new_fd;
    bool delHandler = false;

    sin_size = sizeof(struct sockaddr_in);
    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*)&sin_size)) == -1) 
    {
        perror("accept");
    }
    addr = inet_ntoa(their_addr.sin_addr);
    port = ntohs(their_addr.sin_port);

    TcpClient* tcpClient = new TcpClient(this, addr, new_fd, messageHandler);
    tcpClients.push_back(tcpClient);
    fdManager.add(new_fd, this);

    std::cout << "server: got connection from " << addr << ", port " << port << "\n";

    return delHandler;
}

// inform old clients about the new one
// inform the new client about all the old ones
void Server::sendNewClientAll(TcpClient* newTcpClient)
{
    std::string newPacket = newTcpClient->encodeNewClient();

    for (TcpClients::iterator i = tcpClients.begin(); i != tcpClients.end(); ++i) 
    {
        TcpClient* oldTcpClient = *i;
        if (newTcpClient != oldTcpClient) 
        {
            std::string oldPacket = oldTcpClient->encodeNewClient();
            newTcpClient->send(oldPacket);
            oldTcpClient->send(newPacket);
        }
    }
}

void Server::sendPacketAll(std::string packet)
{
    std::for_each(tcpClients.begin(), tcpClients.end(), std::bind2nd(std::mem_fun(&TcpClient::send), packet));
}

void Server::sendStartRaceAll()
{
    std::ostringstream ostr;
    MsgStartRace msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message to all\n:";
    msg.print();
#endif

    sendPacketAll(ostr.str());
}

void Server::sendStopRaceAll()
{
    std::ostringstream ostr;
    MsgStopRace msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending messageto all\n:";
    msg.print();
#endif

    sendPacketAll(ostr.str());
}

void Server::sendEventInfoAll()
{
    std::for_each(tcpClients.begin(), tcpClients.end(), std::mem_fun(&TcpClient::sendEventInfo));
}

Server::Server(int _port, FdManager& _fdManager, MessageHandler* mh)
    : fdManager(_fdManager), port(_port), serverExiting(false), messageHandler(mh)
{
    try
    {
        tcpListener = new TcpListener(port);
        int sockfd = tcpListener->getSocket();
        fdManager.add(sockfd, this);
        clientsListSize = 0;
        clientsList = 0;
    }
    catch (TcpListenerError&)
    {
        throw;
    }
}

Server::~Server()
{
    int sockfd = tcpListener->getSocket();
    fdManager.remove(sockfd);
    delete tcpListener;
}

void Server::setExiting(bool exiting)
{
    serverExiting = exiting;
}

bool Server::getExiting() const
{
    return serverExiting;
}

Server::TcpClients& Server::getClients()
{
    return tcpClients;
}

int Server::getClientCount()
{
    return tcpClients.size();
}

void Server::kickClient(int index)
{
    TcpClient* tcpClient = tcpClients.at(index);
    tcpClient->sendKick();
}

void Server::kickAllClients()
{
    std::for_each(tcpClients.begin(), tcpClients.end(), std::mem_fun(&TcpClient::sendKick));
}

bool Server::handle(int sockfd)
{
    bool delHandler = false;
    if (sockfd == tcpListener->getSocket())
    {
        delHandler = fromTcpListener(sockfd);
    }
    else
    {
        TcpClients::iterator i =
		std::find_if(tcpClients.begin(), tcpClients.end(),
		    compare_fun(&TcpClient::getSocket, sockfd));
        if (i != tcpClients.end())
            delHandler = fromTcpClient(sockfd, *i);
        else
            std::cout << "Cannot find tcpClient\n";
    }
    return delHandler;
}

void Server::addClientObserver(ClientObserver* clientObserver)
{
    clientObservers.push_back(clientObserver);
}

void Server::removeClientObserver(ClientObserver* clientObserver)
{
    ClientObservers::iterator i =
		std::remove(clientObservers.begin(), clientObservers.end(), clientObserver);
    if (i != clientObservers.end())
    {
        clientObservers.erase(i, clientObservers.end());
    }
    else
    {
        std::cout << "Server::removeClientObserver: could not remove observer\n";
    }
}

void Server::notifyClientObservers()
{
    std::for_each(clientObservers.begin(), clientObservers.end(), 
		  std::bind2nd(std::mem_fun(&ClientObserver::update), clientsList));
}

void Server::freeClientsList()
{
    for (int i = 0; i < clientsListSize; i++)
        delete clientsList[i];

    delete clientsList;
    clientsListSize = 0;
    clientsList = 0;
}

void Server::updateClientsView()
{
    if (clientsList != 0)
	freeClientsList();

    clientsList = new char*[tcpClients.size() + 1];
    clientsListSize = 0;
    sort(tcpClients.begin(), tcpClients.end());
    for (TcpClients::const_iterator i = tcpClients.begin(); i != tcpClients.end(); ++i)
    {
        TcpClient* tcpClient = *i;
        std::string str = tcpClient->toString();
        const char* clientName = str.c_str();
        clientsList[clientsListSize] = new char[strlen(clientName)+1];
        strcpy(clientsList[clientsListSize++], clientName);
    }
    clientsList[clientsListSize] = 0;
}

char** Server::getClientsList()
{
    return clientsList;
}

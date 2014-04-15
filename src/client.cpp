#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <strings.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "database.h"
#include "client.h"
#include "util.h"

#if 1
#define MSGDEBUG
#endif



/************ UDPCLIENT **************************************/

UdpClient::UdpClient(int _id, std::string _ip, 
		     int _port, std::string _name)
    : id(_id), name(_name), ip(_ip), port(_port), 
      location(0.0,0.0,0.0,0.0,0.0,0.0), state(generating), punchedControl(0)
{
}


void UdpClient::setUdpServer(UdpServer* udpServer1)
{
    udpServer = udpServer1;
}

void UdpClient::reset()
{
    Course* course = Database::instance().getEventCourse();

    location.horizontal = course->directionToNextControl(0);
    location.vertical = PI/2;
    location.position = course->controlPosition(0);
}

Point UdpClient::getPosition()
{
    return location.position;
}

fptype UdpClient::getRunDirection()
{
    return location.runDirection;
}

void UdpClient::drawToTerrain()
{
    glPushMatrix();
    /* modeling transformation */

    GLfloat x = (GLfloat)location.position.x;
    GLfloat y = (GLfloat)location.position.y;
    GLfloat z = (GLfloat)location.position.z;
    GLfloat h = Rad2Deg(location.horizontal);
    GLfloat v = Rad2Deg(location.vertical); 
#if 0
    hor[index] = h;
    ver[index] = v;
    pos[index*3+0] = x; pos[index*3+1] = y; pos[index*3+2] = z;
#endif
    glTranslatef(x, y, z);
    glRotatef(h, 0.0, 0.0, 1.0);
    glRotatef(v, 1.0, 0.0, 0.0);

    glScalef(0.05, 0.05, 0.05);

#if 0    
    if (self->night) 
    {
	/* draw headlight's light */
	GLfloat headlight_position[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat headlight_direction[] = {0.0, 0.0, -1.0};
	glLightfv(self->light, GL_POSITION, headlight_position);
	glLightfv(self->light, GL_SPOT_DIRECTION, headlight_direction);
    }
#endif

    glPushAttrib(GL_POLYGON_BIT);
    glDisable(GL_CULL_FACE);
    glutSolidSphere(3.0, 10, 10);
    glPopAttrib();

    glPopMatrix();
}

void UdpClient::getCoordinates(GLfloat* pos, GLfloat* hor, GLfloat* ver, int index)
{
	GLfloat x = -(GLfloat)location.position.x;
	GLfloat y = -(GLfloat)location.position.y;
	GLfloat z = -(GLfloat)location.position.z;
	GLfloat h = -location.horizontal;
	GLfloat v = -location.vertical; 
	hor[index] = h;
	ver[index] = v;
	pos[index*3+0] = x; pos[index*3+1] = y; pos[index*3+2] = z;
}

void UdpClient::drawToMap()
{
    Point pos = getPosition();
    fptype angle = getRunDirection();

    glPointSize(3.0);
    glColor3f(1.0, 0.0, 0.0); // red

    glPushMatrix();

    // modeling transformation
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(Rad2Deg(angle), 0.0, 0.0, 1.0);

    // draw a point indicating orienteer's position
    glBegin(GL_POINTS);
    glVertex2f(0.0, 0.0);
    glEnd();

    // draw a line showing the direction of view
    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, 10.0);
    glEnd();

    glPopMatrix();
}

/* this function gets called when:
 * - we have to draw ourselves into the player view
 * - we have to send new coordinates to peer
 */
int UdpClient::drawToTerrain(Location* location)
{
    static GLint time0 = 0;
    GLint time = glutGet(GLUT_ELAPSED_TIME);
    int frequency = 100; // do sending once per this many milliseconds
    if (time - time0 > frequency) 
    {
        // send updated coordinates to peer
        udpServer->sendMsgPosition(this, location);
        time0 = time;
    }

    drawToTerrain();
    return 0;
}

int UdpClient::drawToTerrainDay(Location* location)
{
    static GLint time0 = 0;
    GLint time = glutGet(GLUT_ELAPSED_TIME);
    int frequency = 100; // do sending once per this many milliseconds
    if (time - time0 > frequency) 
    {
        // send updated coordinates to peer
        udpServer->sendMsgPosition(this, location);
        time0 = time;
    }

    drawToTerrain();
    return 0;
}

int UdpClient::drawToTerrainNight(Location* location)
{
    static GLint time0 = 0;
    GLint time = glutGet(GLUT_ELAPSED_TIME);
    int frequency = 100; // do sending once per this many milliseconds
    if (time - time0 > frequency) 
    {
        // send updated coordinates to peer
        udpServer->sendMsgPosition(this, location);
        time0 = time;
    }

    drawToTerrain();
    return 0;
}

int UdpClient::drawToMap(Location* location)
{
    drawToMap();
    return 0;
}

UdpClient::~UdpClient()
{
}

void UdpClient::setLocation(fptype x, fptype y, fptype z,
			    fptype horizontal, fptype vertical)
{
    location.position.x = x;
    location.position.y = y;
    location.position.z = z;
    location.horizontal = horizontal;
    location.vertical = vertical;
}

std::string UdpClient::stateToString() const 
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

std::string UdpClient::toString() const
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

int UdpClient::print()
{
    std::string str = toString();
    std::cout << str << "\n";
    return 0;
}

void UdpClient::punch(int controlNumber)
{
    punchedControl = controlNumber;
}

bool UdpClient::operator<(const UdpClient& other)
{
    return punchedControl < other.punchedControl;
}

/*********** UDPSERVER ***************************************/

void UdpServer::init(int portStart)
{
    struct sockaddr_in my_addr; // my udp address

    // the lower and upper range of port we are trying to bind to
    unsigned short minPort = portStart; 
    unsigned short maxPort = minPort + 100;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
#ifdef WIN32
    memset(&(my_addr.sin_zero), 0, 8);
#else
    bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */
#endif
    for (port = minPort; port <= maxPort; port++) 
    {
        my_addr.sin_port = htons(port);   /* short, network byte order */
        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
        {
            std::cout << "Client: cannot bind to port " << port << "\n";
        }
        else 
        {
            break; // we found a port to bind to
        }
    }
    std::cout << "Client: receiving messages at port " << port << "\n";
}

UdpServer::UdpServer(Client* _client, int portStart, MessageHandler* mh)
    : client(_client), messageHandler(mh)
{
    init(portStart);
}

UdpServer::~UdpServer()
{
}

void UdpServer::send(UdpClient* receiver, std::string& message) const
{
    std::string ip = receiver->getIp();
    int port = receiver->getPort();

    struct sockaddr_in recv_addr;
    recv_addr.sin_family = AF_INET;   // host byte order
    recv_addr.sin_port = htons(port); // short, network byte order
    recv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
#ifdef WIN32
    memset(&(recv_addr.sin_zero), 0, 8);
#else
    bzero(&(recv_addr.sin_zero), 8);  // zero the rest of the struct
#endif

    const char* msg = message.c_str();
    if (sendto(sockfd, msg, strlen(msg), 0,
	       (struct sockaddr *)&recv_addr, sizeof(struct sockaddr)) == -1) 
    {
        perror("udpServerSend");
    }
    //printf("sent message (%s) to %s, port %d\n", msg, ip, port);
}

bool UdpServer::receive(std::string buf)
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
            std::cout << "UdpServer received message:\n";
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
    catch  (MessageNotReady&)
    {
    }
    return delHandler;
}

void UdpServer::sendMsgPosition(UdpClient* udpClient, Location* location)
{
    std::string message = MsgPosition::enc(id, location);
    send(udpClient, message);
}

/******** TCPSERVER ******************************************/

/*
 * Assuming the server and client are on the same host,
 * which is katmai (192.168.0.102), we want all of the
 * following to translate to 192.168.0.102
 *
 * localhost -> 127.0.0.1
 * 127.0.0.1 -> 127.0.0.1
 * katmai    -> 127.0.0.1
 * 192.168.0.102 -> 192.168.0.102
 */
bool TcpServer::connect(std::string address, int _port)
{
    struct hostent *he;
    struct sockaddr_in their_addr; // server tcp address
    char hname[128];

    if (address == "localhost" || address == "127.0.0.1")
    {
        gethostname(hname, 128);
        hostname = hname;
    } 
    else 
    {
        hostname = address;
    }

    port = _port;

    if ((he = gethostbyname(hostname.c_str())) == NULL)
    { 
        /* get the host info */
        perror("gethostbyname");
        return connected = false;
    }

    // create a socket to connect to server
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        return connected = false;
    }
  
    their_addr.sin_family = AF_INET;      // host byte order
    their_addr.sin_port = htons(port); // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
#ifdef WIN32
    memset(&(their_addr.sin_zero), 0, 8);
#else
    bzero(&(their_addr.sin_zero), 8);     // zero the rest of the struct
#endif
  
    if (::connect(sockfd, (struct sockaddr *)&their_addr, 
		  sizeof(struct sockaddr)) == -1) 
    {
        perror("connect");
        return connected = false;
    }
    return connected = true;
}

void TcpServer::disconnect()
{
    if (isConnected()) 
    {
#ifdef WIN32
      	closesocket(sockfd);
#else
      	close(sockfd);
#endif
	connected = false;
    }
}

TcpServer::TcpServer(Client* _client, MessageHandler* mh)
    : hostname(""), port(0), sockfd(-1), connected(false), 
      client(_client), messageHandler(mh)
{
}

TcpServer::~TcpServer()
{
    disconnect();
}

void TcpServer::send(std::string message) const
{
    const char* msg = message.c_str();
    if (::send(sockfd, msg, strlen(msg), 0) == -1) {
        perror("tcpServerSend");
    }
    std::cout << "TcpServer: Sent packet: " << message << "\n";
}

bool TcpServer::receive(std::string buf)
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
	    std::cout << "TcpServer received message:\n";
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
    catch  (MessageNotReady&)
    {
    }
    return delHandler;
}

void TcpServer::sendDiscReq()
{
    std::ostringstream ostr;
    MsgDiscReq msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());
}

void TcpServer::sendJoinReq(std::string myName, int port)
{
    std::ostringstream ostr;
    MsgJoinReq msg(myName, port);
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());
}

void TcpServer::sendReadyToGo()
{
    std::ostringstream ostr;
    MsgReadyToGo msg;
    msg.encode(ostr);

#ifdef MSGDEBUG
    std::cout << "Sending message\n:";
    msg.print();
#endif

    send(ostr.str());
}

/*********** CLIENT ***************************************/

// udp client receives a message from another udp client
bool Client::fromUdpPeer(int sockfd)
{
#define MAXDATASIZE 4000
    char buf[MAXDATASIZE];
    bool delHandler = false;
    // receive a message, don't care who it is from
    int nbytes = recvfrom(sockfd, buf, MAXDATASIZE, 0, NULL, NULL);

    if (nbytes < 0) 
    {
        printf("Error reading from udp socket\n");
    }
    else if (nbytes == 0) 
    {
        ; // what does it mean to get zero bytes on udp socket ???
    }
    else 
    {
        buf[nbytes] = '\0';
        delHandler = udpServer->receive(buf);
    }
    return delHandler;
}

bool Client::fromTcpServer(int sockfd)
{
    // we have a message from the server
    char buf[MAXDATASIZE];
    bool delHandler = false;
    int nbytes = recv(sockfd, buf, MAXDATASIZE, 0);

    if (nbytes < 0) 
    {
        printf("Error reading tcp socket\n");
    }
    else if (nbytes == 0) 
    {
        printf("Server has closed connection\n");
	// create an artificial MsgDiscConf to do the normal disconnect
	std::ostringstream ostr;
	MsgDiscConf msg;
	msg.encode(ostr);
	strcpy(buf, ostr.str().c_str());
        delHandler = tcpServer->receive(buf);
    }
    else 
    {
        buf[nbytes] = '\0';
        delHandler = tcpServer->receive(buf);
    }
    return delHandler;
}

void Client::printUdpClients()
{
    std::cout << "Current clients are:\n";
    std::for_each(udpClients.begin(), udpClients.end(), std::mem_fun(&UdpClient::print));
    std::cout << "End of clients\n";
}

void Client::deleteAllUdpClients()
{
    std::for_each(udpClients.begin(), udpClients.end(), del_fun<UdpClient>());
    udpClients.erase(udpClients.begin(), udpClients.end());
}

void Client::connect(std::string hostname, int tcpPort)
{
    if (tcpServer->connect(hostname, tcpPort)) 
    {
        int tcpSocket = tcpServer->getSocket();
        fdManager.add(tcpSocket, this);
    }
    else
    {
        throw ConnectError();
    }
}

void Client::disconnectTcp()
{
#if 0
    int sockfd = tcpServer->getSocket();
    fdManager.remove(sockfd);
#endif
    tcpServer->disconnect();
    deleteAllUdpClients();
}

void Client::addUdpClient(UdpClient* udpClient)
{
    udpClient->setUdpServer(udpServer);
    udpClient->reset();
    udpClients.push_back(udpClient);
}

UdpClient* Client::udpClientById(int udpClientId)
{
    UdpClients::iterator i =
		std::find_if(udpClients.begin(), udpClients.end(), compare_fun(&UdpClient::getId, udpClientId));
    return (i != udpClients.end()) ? *i : 0;
}

UdpClient* Client::removeUdpClient(int id)
{
    UdpClient* udpClient = udpClientById(id);
    if (udpClient != 0)
    {
        UdpClients::iterator i = 
		    std::remove(udpClients.begin(), udpClients.end(), udpClient);
        udpClients.erase(i, udpClients.end());
    }
    else
    {
        std::cout << "UdpClient " << id << " not found\n";
    }
    return udpClient;
}

// Begin SocketHandler implementation
bool Client::handle(int sockfd)
{
    bool delHandler = false;
    if (sockfd == tcpServer->getSocket())
    {
        delHandler = fromTcpServer(sockfd);
    }
    else
    {
        delHandler = fromUdpPeer(sockfd);
    }
    return delHandler;
}
// End SocketHandler implementation


Client::Client(int startport, FdManager& _fdManager, MessageHandler* mh)
    : fdManager(_fdManager), messageHandler(mh)
{
    
    udpServer = new UdpServer(this, startport, messageHandler);
    int udpSocket = udpServer->getSocket();
    port = udpServer->getPort();
    tcpServer = new TcpServer(this, messageHandler);
    fdManager.add(udpSocket, this);
    clientsListSize = 0;
    clientsList = 0;
}

Client::~Client()
{
    int udpSocket = udpServer->getSocket();

    fdManager.remove(udpSocket);

    deleteAllUdpClients();
    delete udpServer;
    delete tcpServer;
}

void Client::sendJoinReq(std::string name1)
{
    name = name1;
    tcpServer->sendJoinReq(name, udpServer->getPort());
}

void Client::sendAll(std::string& packet) const 
{
    for (UdpClients::const_iterator i = udpClients.begin(); i != udpClients.end(); ++i) 
    {
        UdpClient* receiver = *i;
        udpServer->send(receiver, packet);
    }
}

#if 0
void Client::sendRunAll(Run* run)
{
    std::string packet = MsgRun::enc(udpServer->getId(), run);
    sendAll(packet);
}
#endif

void Client::sendChat(std::string buf)
{
    std::string packet = MsgChat::enc(udpServer->getId(), buf);
    sendAll(packet);
}

void Client::sendChat(int index, std::string buf)
{
    std::string packet = MsgChat::enc(udpServer->getId(), buf);
    UdpClient* receiver = udpClients.at(index);
    udpServer->send(receiver, packet);
}

void Client::sendReadyToGo() const
{
    tcpServer->sendReadyToGo(); 
}

void Client::sendPunch(int controlNumber) const
{
    std::string packet = MsgPunch::enc(udpServer->getId(), controlNumber);
    sendAll(packet);
    tcpServer->send(packet);
}

void Client::sendFinished(fptype totalTime) const
{
    std::string packet = MsgFinished::enc(udpServer->getId(), totalTime);
    sendAll(packet);
    tcpServer->send(packet);
}

void Client::sendQuit() const
{
    std::string packet = MsgQuit::enc(udpServer->getId());
    sendAll(packet);
    tcpServer->send(packet);
}

void Client::sendCheat() const 
{
    std::string packet = MsgCheat::enc(udpServer->getId());
    sendAll(packet);
    tcpServer->send(packet);
}

void Client::addClientObserver(ClientObserver* clientObserver)
{
    clientObservers.push_back(clientObserver);
}

void Client::removeClientObserver(ClientObserver* clientObserver)
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

void Client::notifyClientObservers()
{
    std::for_each(clientObservers.begin(), clientObservers.end(), 
		  std::bind2nd(std::mem_fun(&ClientObserver::update), clientsList));
}

void Client::freeClientsList()
{
    for (int i = 0; i < clientsListSize; i++)
        delete clientsList[i];

    delete clientsList;
    clientsListSize = 0;
    clientsList = 0;
}

void Client::updateClientsView()
{
    if (clientsList != 0)
        freeClientsList();

    clientsList = new char*[udpClients.size() + 1];
    clientsListSize = 0;
    sort(udpClients.begin(), udpClients.end());
    for (UdpClients::const_iterator i = udpClients.begin(); i != udpClients.end(); ++i)
    {
        UdpClient* udpClient = *i;
        std::string str = udpClient->toString();
        const char* clientName = str.c_str();
        clientsList[clientsListSize] = new char[strlen(clientName)+1];
        strcpy(clientsList[clientsListSize++], clientName);
    }
    clientsList[clientsListSize] = 0;
}

char** Client::getClientsList()
{
    return clientsList;
}

#ifndef _MESSAGE_H
#define _MESSAGE_H


#include <istream>
#include <string>
#include <map>
#include <stdexcept>

#include "terrain.h"
#include "course.h"
#include "map.h"
#include "event.h"
#include "run.h"

class TcpClient;
class TcpServer;
class UdpServer;

class MsgJoinReq;
class MsgJoinAccept;
class MsgJoinReject;
class MsgEventInfo;
class MsgReadyToGo;
class MsgStartRace;
class MsgPunch;
class MsgFinished;
class MsgQuit;
class MsgCheat;
class MsgStopRace;
class MsgNewClient;
class MsgKick;
class MsgDiscReq;
class MsgDiscConf;
class MsgRemoveClient;
class MsgChat;
class MsgPosition;
class MsgRun;


class MessageHandler
{
 public:
    // messages send by the tcp server (from server to client)
    virtual bool handle(const MsgJoinAccept& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgJoinReject& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgEventInfo& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgStartRace& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgStopRace& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgNewClient& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgKick& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgDiscConf& msg, TcpServer* fromTcpServer) = 0;
    virtual bool handle(const MsgRemoveClient& msg, TcpServer* fromTcpServer) = 0;

    // messages sent by the tcp client (from client to server)
    virtual bool handle(const MsgJoinReq& msg, TcpClient* fromTcpClient) = 0;
    virtual bool handle(const MsgReadyToGo& msg, TcpClient* fromTcpClient) = 0;
    virtual bool handle(const MsgPunch& msg, TcpClient* fromTcpClient) = 0;
    virtual bool handle(const MsgFinished& msg, TcpClient* fromTcpClient) = 0;
    virtual bool handle(const MsgQuit& msg, TcpClient* fromTcpClient) = 0;
    virtual bool handle(const MsgCheat& msg, TcpClient* fromTcpClient) = 0;
    virtual bool handle(const MsgDiscReq& msg, TcpClient* fromTcpClient) = 0;

    // messages sent by the udp server (from client to client)
    virtual bool handle(const MsgChat& msg, UdpServer* fromUdpServer) = 0;
    virtual bool handle(const MsgPosition& msg, UdpServer* fromUdpServer) = 0;
    virtual bool handle(const MsgPunch& msg, UdpServer* fromUdpServer) = 0;
    virtual bool handle(const MsgFinished& msg, UdpServer* fromUdpServer) = 0;
    virtual bool handle(const MsgQuit& msg, UdpServer* fromUdpServer) = 0;
    virtual bool handle(const MsgCheat& msg, UdpServer* fromUdpServer) = 0;

    virtual ~MessageHandler() {}
};


class MessageNotReady : public std::runtime_error 
{
 public:
    MessageNotReady () : std::runtime_error("MessageNotReady") { }
};


class MessageBuffer
{
 private:
    std::string buffer;

 public:
    void append(std::string str);
    std::string getMessage();
};

struct Message
{
    typedef Message* (*Decoder)(std::istream&);

    typedef std::map<int, Decoder> DecodersMap;
    //typedef std::vector<MessageHandler*> Handlers;

    virtual void encode(std::ostream& ostr) const;
    virtual bool handle(MessageHandler* mh, TcpClient* tcpClient) const;
    virtual bool handle(MessageHandler* mh, TcpServer* tcpServer) const;
    virtual bool handle(MessageHandler* mh, UdpServer* udpServer) const;
    virtual int msgId() const = 0;
    virtual void print() const = 0;
    virtual ~Message();

    //static Handlers handlers;
    static DecodersMap& getDecoders();
    static Message* decode(std::istream& istr);
};

class MsgJoinReq : public Message
{
 private:
    std::string name;
    int         port; // udp port this client will be listening to

    static MessageHandler* handler;

 public:
    MsgJoinReq(std::string _name, int _port) : name(_name), port(_port) {}

    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    void print() const;
    int msgId() const { return 1; }
    std::string getName() const { return name; }
    int getPort() const { return port; }
};

class MsgJoinAccept : public Message
{
 private:
    int id; // the id of the joined client, assigned by server

 public:
    MsgJoinAccept(int _id) : id(_id) {}
    virtual ~MsgJoinAccept();
    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* tcpServer) const;
    void print() const;
    int msgId() const { return 2; }
    int getId() const { return id; }
};

class MsgJoinReject : public Message
{
 public:
    MsgJoinReject() {}
    virtual ~MsgJoinReject();
    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;
    int msgId() const { return 3; }
};

class MsgEventInfo : public Message
{
 private:
    Terrain* terrain;
    Map* map;
    Course* course;
    Event* event;

 public:
    MsgEventInfo(Terrain* _terrain, Map* _map, Course* _course, Event* _event)
	: terrain(_terrain), map(_map), course(_course), event(_event) {}
    virtual ~MsgEventInfo();
    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;
    int msgId() const { return 4; }
    Terrain* getTerrain() const { return terrain; }
    Map* getMap() const { return map; }
    Course* getCourse() const { return course; }
    Event* getEvent() const { return event; }
};

class MsgReadyToGo : public Message
{
 public:
    MsgReadyToGo() {}
    virtual ~MsgReadyToGo();
    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    void print() const;
    int msgId() const { return 5; }
};

class MsgStartRace : public Message
{
 public:
    MsgStartRace() {}
    virtual ~MsgStartRace();
    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;
    int msgId() const { return 6; }
};

class MsgPunch : public Message
{
 private:
    int id;            // id of the client sending this
    int controlNumber;

 public:
    MsgPunch(int _id, int _controlNumber) : id(_id), controlNumber(_controlNumber) {}
    virtual ~MsgPunch();

    int getId() const { return id; }

    int getControlNumber() const { return controlNumber; }
    static std::string enc(int id, int controlNumber);
    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;
    int msgId() const { return 7; }
};

class MsgFinished : public Message
{
 private:
    int id;            // id of the client sending this
    fptype totalTime; // unit: seconds

 public:
    MsgFinished(int _id, fptype tt) : id(_id), totalTime(tt) {}
    ~MsgFinished();

    int getId() const { return id; }

    fptype getTotalTime() const { return totalTime; }
    void encode(std::ostream& ostr);
    static std::string enc(int id, fptype totalTime);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;
    int msgId() const { return 8; }
};

class MsgQuit : public Message
{
 private:
    int id;            // id of the client sending this

 public:
    MsgQuit(int _id) : id(_id) {}
    ~MsgQuit();

    int getId() const { return id; }

    void encode(std::ostream& ostr);
    static std::string enc(int id);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;
    int msgId() const { return 9; }
};

class MsgCheat : public Message
{
 private:
    int id;            // id of the client sending this

 public:
    MsgCheat(int _id) : id(_id) {}
    ~MsgCheat();

    int getId() const { return id; }
    void encode(std::ostream& ostr);
    static std::string enc(int id);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;

    int msgId() const { return 10; }
};

class MsgStopRace : public Message
{
 private:
    // empty
 public:
    MsgStopRace() {}
    ~MsgStopRace();

    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;

    int msgId() const { return 11; }
};

class MsgNewClient : public Message
{
 private:
    int id;           // id of the new client
    std::string ip;   // ip address of the new client
    int port;         // the port this client is listening to for udp messages
    std::string name; // name of this client

 public:
    MsgNewClient(int _id, std::string _ip, int _port, std::string _name)
	: id(_id), ip(_ip), port(_port), name(_name)
	{}
    ~MsgNewClient();

    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;
    int msgId() const { return 12; }

    int getId() const { return id; }
    std::string getIp() const { return ip; }
    int getPort() const { return port; }
    std::string getName() const { return name; }
};

class MsgKick : public Message
{
 private:
    // empty
 public:
    MsgKick() {}
    ~MsgKick();

    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;
    int msgId() const { return 13; }
};

class MsgDiscReq : public Message
{
 private:
    // empty
 public:
    MsgDiscReq() {}
    ~MsgDiscReq();

    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpClient* fromTcpClient) const;
    void print() const;

    int msgId() const { return 14; }
};

class MsgDiscConf : public Message
{
 private:
    // empty
 public:
    MsgDiscConf() {}
    ~MsgDiscConf();
    
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;

    int msgId() const { return 15; }
};

class MsgRemoveClient : public Message
{
 private:
    int id; // id of the client to be removed
 public:
    MsgRemoveClient(int _id) : id(_id) {}
    ~MsgRemoveClient();

    void encode(std::ostream& ostr);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, TcpServer* fromTcpServer) const;
    void print() const;

    int msgId() const { return 16; }
    int getId() const { return id; }
};

class MsgChat : public Message
{
 private:
    int id;                // id of the client sending this
    std::string message;   // the message
 public:
    MsgChat(int _id, std::string bf) : id(_id), message(bf) {}
    ~MsgChat();

    int getId() const { return id; }
    std::string getMessage() const { return message; }
    
    void encode(std::ostream& ostr);
    static std::string enc(int id, std::string buf);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;
    int msgId() const { return 17; }
};

class MsgPosition : public Message
{
 private:
    int id;            // id of the client sending this
    fptype x, y, z;    // new position (x,y, z) of the client
    fptype horizontal; // direction client is looking at
    fptype vertical;   // direction client is looking at
 public:
    MsgPosition(int _id, fptype _x, fptype _y, fptype _z,
		fptype h, fptype v) 
	: id(_id), x(_x), y(_y), z(_z), horizontal(h), vertical(v)
    {}
    ~MsgPosition();
    int getId() const { return id; }
    fptype getX() const { return x; }
    fptype getY() const { return y; }
    fptype getZ() const { return z; }
    fptype getHorizontal() const { return horizontal; }
    fptype getVertical() const { return vertical; }
    
    void encode(std::ostream& ostr);
    static std::string enc(int id, Location* location);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;
    int msgId() const { return 18; }
};

#if 0
class MsgRun : public Message
{
 private:
    int id;
    Run* run;

 public:
    MsgRun(int _id, Run* _run) : id(_id), run(_run) {}
    MsgRun::~MsgRun();

    int getId() const { return id; }
    Run* getRun() const { return run; }
    
    void encode(std::ostream& ostr);
    static std::string enc(int id, Run* _run);
    static Message* decode(std::istream& istr);
    bool handle(MessageHandler* mh, UdpServer* fromUdpServer) const;
    void print() const;
    int msgId() const { return 19; }
};
#endif


#endif // _MESSAGE_H

#include <string>
#include <iostream>
#include <ostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iomanip>

#include "orienteer.h"
#include "database.h"
#include "message.h"
#include "xmlParser.h"


// len msgid id ip port name // circular buffer strips len
// msgid id ip port name     // client dispatcher strips msgid
// id ip port name           // message decodes the rest

using batb::XmlParser;


//Message::Handlers Message::handlers;

Message::DecodersMap& Message::getDecoders()
{
    static Message::DecodersMap* decoders = new Message::DecodersMap;

    (*decoders)[ 1] = MsgJoinReq::decode;
    (*decoders)[ 2] = MsgJoinAccept::decode;
    (*decoders)[ 3] = MsgJoinReject::decode;
    (*decoders)[ 4] = MsgEventInfo::decode;
    (*decoders)[ 5] = MsgReadyToGo::decode;
    (*decoders)[ 6] = MsgStartRace::decode;
    (*decoders)[ 7] = MsgPunch::decode;
    (*decoders)[ 8] = MsgFinished::decode;
    (*decoders)[ 9] = MsgQuit::decode;
    (*decoders)[10] = MsgCheat::decode;
    (*decoders)[11] = MsgStopRace::decode;
    (*decoders)[12] = MsgNewClient::decode;
    (*decoders)[13] = MsgKick::decode;
    (*decoders)[14] = MsgDiscReq::decode;
    (*decoders)[15] = MsgDiscConf::decode;
    (*decoders)[16] = MsgRemoveClient::decode;
    (*decoders)[17] = MsgChat::decode;
    (*decoders)[18] = MsgPosition::decode;

    return *decoders;
}

bool Message::handle(MessageHandler* mh, TcpClient* fromTcpClient) const
{
    std::cout << "No handler defined\n";
    return false;
}

bool Message::handle(MessageHandler* mh, TcpServer* fromTcpServer) const
{
    std::cout << "No handler defined\n";
    return false;
}

bool Message::handle(MessageHandler* mh, UdpServer* fromUdpServer) const
{
    std::cout << "No handler defined\n";
    return false;
}

Message* Message::decode(std::istream& istr)
{
    static Message::DecodersMap decoders = Message::getDecoders();
    int classTag;
    istr >> classTag;
    Decoder decoder = decoders[classTag];
    return decoder(istr);
}

void Message::encode(std::ostream& ostr) const
{
    std::ostringstream str;
    str << " " << msgId();
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message::~Message()
{
}

//////////////////////////////////////////////////

void MsgJoinReq::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() << " " << name << " " << port;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s; // "0015 1 foo 1234"
}

Message* MsgJoinReq::decode(std::istream& istr)
{
    std::string name;
    int port;
    istr >> name >> port;
    return new MsgJoinReq(name, port);
}

void MsgJoinReq::print() const
{
    std::cout << "MsgJoinReq: Name: " << name << " Port: " << port << std::endl;
}

bool MsgJoinReq::handle(MessageHandler* mh, TcpClient* tcpClient) const 
{
    return mh->handle(*this, tcpClient); 
}

//////////////////////////////////////////////////

MsgJoinAccept::~MsgJoinAccept()
{
}

void MsgJoinAccept::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() << " " << id;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgJoinAccept::decode(std::istream& istr)
{
    int id;
    istr >> id;
    return new MsgJoinAccept(id);
}

void MsgJoinAccept::print() const
{
    std::cout << "MsgJoinAccept: id: " << id << std::endl;
}

bool MsgJoinAccept::handle(MessageHandler* mh, TcpServer* tcpServer) const 
{
    return mh->handle(*this, tcpServer); 
}

//////////////////////////////////////////////////

MsgJoinReject::~MsgJoinReject()
{
}

void MsgJoinReject::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId();
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgJoinReject::decode(std::istream& istr)
{
    return new MsgJoinReject();
}

void MsgJoinReject::print() const
{
    std::cout << "MsgJoinReject" << std::endl;
}

bool MsgJoinReject::handle(MessageHandler* mh, TcpServer* fromTcpServer) const 
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgReadyToGo::~MsgReadyToGo()
{
}

void MsgReadyToGo::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId();
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgReadyToGo::decode(std::istream& istr)
{
    return new MsgReadyToGo();
}

void MsgReadyToGo::print() const
{
    std::cout << "MsgReadyToGo" << std::endl;
}

bool MsgReadyToGo::handle(MessageHandler* mh, TcpClient* fromTcpClient) const 
{
    return mh->handle(*this, fromTcpClient); 
}

//////////////////////////////////////////////////

MsgEventInfo::~MsgEventInfo()
{
}

void MsgEventInfo::encode(std::ostream& ostr)
{
    std::ostringstream str;

    str << " " << msgId() << " ";
    terrain->toXML(str, 0);
    map->toXML(str, 0);
    course->toXML(str, 0);
    event->toXML(str, 0);
    
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgEventInfo::decode(std::istream& istr)
{
    MsgEventInfo* msgEventInfo = 0;
    const XmlParser& xmlParser = XmlParser::instance();
    xmlParser.msgEventInfoXMLparse(istr, msgEventInfo);
    return msgEventInfo;
}

void MsgEventInfo::print() const
{
    std::cout << "MsgEventInfo" << std::endl;
}

bool MsgEventInfo::handle(MessageHandler* mh, TcpServer* fromTcpServer) const 
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgStartRace::~MsgStartRace()
{
}

void MsgStartRace::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId();
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgStartRace::decode(std::istream& istr)
{
    return new MsgStartRace();
}

void MsgStartRace::print() const
{
    std::cout << "MsgStartRace" << std::endl;
}

bool MsgStartRace::handle(MessageHandler* mh, TcpServer* fromTcpServer) const 
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgPunch::~MsgPunch()
{
}

std::string MsgPunch::enc(int id, int controlNumber)
{
    std::ostringstream ostr;
    MsgPunch msg(id, controlNumber);
    msg.encode(ostr);
    return ostr.str();
}

void MsgPunch::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() 
	<< " " << id
	<< " " << controlNumber;

    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgPunch::decode(std::istream& istr)
{
    int id;            // id of the client sending this
    int controlNumber;
    istr >> id >> controlNumber;
    return new MsgPunch(id, controlNumber);
}

void MsgPunch::print() const
{
    std::cout << "MsgPunch: controlNumber: " 
	      << controlNumber << std::endl;
}

bool MsgPunch::handle(MessageHandler* mh, TcpClient* fromTcpClient) const
{
    return mh->handle(*this, fromTcpClient); 
}

bool MsgPunch::handle(MessageHandler* mh, UdpServer* fromUdpServer) const
{
    return mh->handle(*this, fromUdpServer); 
}

//////////////////////////////////////////////////

MsgFinished::~MsgFinished()
{
}

std::string MsgFinished::enc(int id, fptype totalTime)
{
    std::ostringstream ostr;
    MsgFinished msg(id, totalTime);
    msg.encode(ostr);
    return ostr.str();
}

void MsgFinished::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId()
	<< " " << id
	<< " " << totalTime;

    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgFinished::decode(std::istream& istr)
{
    int id;
    fptype totalTime;
    istr >> id >> totalTime;
    return new MsgFinished(id, totalTime);
}

void MsgFinished::print() const
{
    std::cout << "MsgFinished: totalTime: " 
	      << totalTime << std::endl;
}

bool MsgFinished::handle(MessageHandler* mh, TcpClient* fromTcpClient) const
{
    return mh->handle(*this, fromTcpClient); 
}

bool MsgFinished::handle(MessageHandler* mh, UdpServer* fromUdpServer) const
{
    return mh->handle(*this, fromUdpServer); 
}

//////////////////////////////////////////////////

MsgQuit::~MsgQuit()
{
}

void MsgQuit::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId()
	<< " " << id;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

std::string MsgQuit::enc(int id)
{
    std::ostringstream ostr;
    MsgQuit msg(id);
    msg.encode(ostr);
    return ostr.str();
}

Message* MsgQuit::decode(std::istream& istr)
{
    int id;            // id of the client sending this
    istr >> id;
    return new MsgQuit(id);
}

void MsgQuit::print() const
{
    std::cout << "MsgQuit: " << std::endl;
}

bool MsgQuit::handle(MessageHandler* mh, TcpClient* fromTcpClient) const
{
    return mh->handle(*this, fromTcpClient); 
}

bool MsgQuit::handle(MessageHandler* mh, UdpServer* fromUdpServer) const
{
    return mh->handle(*this, fromUdpServer); 
}

//////////////////////////////////////////////////

MsgCheat::~MsgCheat()
{
}

void MsgCheat::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId()
	<< " " << id;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

std::string MsgCheat::enc(int id)
{
    std::ostringstream ostr;
    MsgCheat msg(id);
    msg.encode(ostr);
    return ostr.str();
}

Message* MsgCheat::decode(std::istream& istr)
{
    int id;            // id of the client sending this
    istr >> id;
    return new MsgCheat(id);
}

void MsgCheat::print() const
{
    std::cout << "MsgCheat: " << std::endl;
}

bool MsgCheat::handle(MessageHandler* mh, TcpClient* fromTcpClient) const
{
    return mh->handle(*this, fromTcpClient); 
}

bool MsgCheat::handle(MessageHandler* mh, UdpServer* fromUdpServer) const
{
    return mh->handle(*this, fromUdpServer); 
}

//////////////////////////////////////////////////

MsgStopRace::~MsgStopRace()
{
}

Message* MsgStopRace::decode(std::istream& istr)
{
    return new MsgStopRace();
}

void MsgStopRace::print() const
{
    std::cout << "MsgStopRace: " << std::endl;
}

bool MsgStopRace::handle(MessageHandler* mh, TcpServer* fromTcpServer) const
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgKick::~MsgKick()
{
}

Message* MsgKick::decode(std::istream& istr)
{
    return new MsgKick();
}

void MsgKick::print() const
{
    std::cout << "MsgKick: " << std::endl;
}

bool MsgKick::handle(MessageHandler* mh, TcpServer* fromTcpServer) const
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgDiscReq::~MsgDiscReq()
{
}

Message* MsgDiscReq::decode(std::istream& istr)
{
    return new MsgDiscReq();
}

void MsgDiscReq::print() const
{
    std::cout << "MsgDiscReq: " << std::endl;
}

bool MsgDiscReq::handle(MessageHandler* mh, TcpClient* fromTcpClient) const
{
    return mh->handle(*this, fromTcpClient); 
}

//////////////////////////////////////////////////

MsgDiscConf::~MsgDiscConf()
{
}

Message* MsgDiscConf::decode(std::istream& istr)
{
    return new MsgDiscConf();
}

void MsgDiscConf::print() const
{
    std::cout << "MsgDiscConf: " << std::endl;
}

bool MsgDiscConf::handle(MessageHandler* mh, TcpServer* fromTcpServer) const
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgNewClient::~MsgNewClient()
{
}

void MsgNewClient::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() 
	<< " " << id
	<< " " << ip
	<< " " << port
	<< " " << name;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgNewClient::decode(std::istream& istr)
{
    int id;           // id of the new client
    std::string ip;   // ip address of the new client
    int port;         // the port this client is listening to for udp messages
    std::string name; // name of this client
    istr >> id >> ip >> port >> name;
    return new MsgNewClient(id, ip, port, name);
}

void MsgNewClient::print() const
{
    std::cout << "MsgNewClient: " 
	      << " id " << id
	      << " ip " << ip
	      << " port " << port
	      << " name " << name
	      << std::endl;
}


bool MsgNewClient::handle(MessageHandler* mh, TcpServer* fromTcpServer) const
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgRemoveClient::~MsgRemoveClient()
{
}

void MsgRemoveClient::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() 
	<< " " << id;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgRemoveClient::decode(std::istream& istr)
{
    int id; // id of the client to be removed
    istr >> id;
    return new MsgRemoveClient(id);
}

void MsgRemoveClient::print() const
{
    std::cout << "MsgRemoveClient: " << std::endl;
}

bool MsgRemoveClient::handle(MessageHandler* mh, TcpServer* fromTcpServer) const
{
    return mh->handle(*this, fromTcpServer); 
}

//////////////////////////////////////////////////

MsgChat::~MsgChat()
{
}

std::string MsgChat::enc(int id, std::string buf)
{
    std::ostringstream ostr;
    MsgChat msg(id, buf);
    msg.encode(ostr);
    return ostr.str();
}

void MsgChat::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() 
	<< " " << id
	<< " " << message;
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgChat::decode(std::istream& istr)
{
    int id;         // id of the client sending this message
    std::string bf; // the message
    istr >> id;
    getline(istr, bf);
    return new MsgChat(id, bf);
}

void MsgChat::print() const
{
    std::cout << "MsgChat: " << message << "\n";
}

bool MsgChat::handle(MessageHandler* mh, UdpServer* fromUdpServer) const
{
    return mh->handle(*this, fromUdpServer); 
}

//////////////////////////////////////////////////

MsgPosition::~MsgPosition()
{
}

void MsgPosition::encode(std::ostream& ostr)
{
    std::ostringstream str;
    str << " " << msgId() 
	<< " " << id
	<< " " << x
	<< " " << y
	<< " " << z
	<< " " << horizontal
	<< " " << vertical;

    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

std::string MsgPosition::enc(int id, Location* location)
{
    std::ostringstream ostr;
    MsgPosition msg(id, 
		    location->position.x,
		    location->position.y,
		    location->position.z,
		    location->runDirection,
		    location->vertical);
    msg.encode(ostr);
    return ostr.str();
}

Message* MsgPosition::decode(std::istream& istr)
{
    int id;            // id of the client sending this
    fptype x, y, z;    // new position (x,y, z) of the client
    fptype horizontal; // direction client is looking at
    fptype vertical;   // direction client is looking at
    istr >> id >> x >> y >> z >> horizontal >> vertical;
    return new MsgPosition(id, x, y, z, horizontal, vertical);
}

void MsgPosition::print() const
{
    std::cout << "MsgPosition: " << std::endl;
}

bool MsgPosition::handle(MessageHandler* mh, UdpServer* fromUdpServer) const 
{
    return mh->handle(*this, fromUdpServer); 
}

//////////////////////////////////////////////////
#if 0
MsgRun::~MsgRun()
{
}

std::string MsgRun::enc(int id, Run* run)
{
    std::ostringstream ostr;
    MsgRun msg(id, run);
    msg.encode(ostr);
    return ostr.str();
}

void MsgRun::encode(std::ostream& ostr)
{
    std::ostringstream str;

    str << " " << msgId() 
	<< " " << id 
	<< " ";
    run->toXML(str, 0);
    
    std::string s = str.str();
    int len = s.length()+4;
    ostr << std::setw(4) << len << s;
}

Message* MsgRun::decode(std::istream& istr)
{
    int id;
    MsgRun* msgRun = 0;
    istr >> id;
    const XmlParser& xmlParser = XmlParser::instance();
    xmlParser.msgRunParse(istr, msgRun);
    return msgRun;
}

void MsgRun::print() const
{
    std::cout << "MsgRun: " << std::endl;
}

bool MsgRun::handle(MessageHandler* mh, UdpServer* fromUdpServer) const 
{
    return mh->handle(*this, fromUdpServer); 
}
#endif

//////////////////////////////////////////////////

class Foo : public MessageHandler
{
public:
    bool handle(const MsgJoinReq& msg, void* userData) const;
    bool handle(const MsgJoinAccept& msg, void* userData) const;
    bool handle(const MsgJoinReject& msg, void* userData) const;
    bool handle(const MsgEventInfo& msg, void* userData) const;
    bool handle(const MsgReadyToGo& msg, void* userData) const;
    bool handle(const MsgStartRace& msg, void* userData) const;
    bool handle(const MsgPunch& msg, void* userData) const;
    bool handle(const MsgFinished& msg, void* userData) const;
    bool handle(const MsgQuit& msg, void* userData) const;
    bool handle(const MsgCheat& msg, void* userData) const;
    bool handle(const MsgStopRace& msg, void* userData) const;
    bool handle(const MsgNewClient& msg, void* userData) const;
    bool handle(const MsgKick& msg, void* userData) const;
    bool handle(const MsgDiscReq& msg, void* userData) const;
    bool handle(const MsgDiscConf& msg, void* userData) const;
    bool handle(const MsgRemoveClient& msg, void* userData) const;
    bool handle(const MsgChat& msg, void* userData) const;
    bool handle(const MsgPosition& msg, void* userData) const;
};


bool Foo::handle(const MsgJoinReq& msg, void* userData) const
{
    msg.print();
    return true;
}

bool Foo::handle(const MsgJoinAccept& msg, void* userData) const
{
    msg.print();
    return true;
}

bool Foo::handle(const MsgJoinReject& msg, void* userData) const
{
    msg.print();
    return true;
}

bool Foo::handle(const MsgEventInfo& msg, void* userData) const
{
    msg.print();
    return true;
}

bool Foo::handle(const MsgReadyToGo& msg, void* userData) const
{
    msg.print();
    return true;
}

bool Foo::handle(const MsgStartRace& msg, void* userData) const
{
    msg.print();
    return true;
}

bool Foo::handle(const MsgPunch& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgFinished& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgQuit& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgCheat& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgStopRace& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgNewClient& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgKick& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgDiscReq& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgDiscConf& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgRemoveClient& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgChat& msg, void* userData) const
{ 
    msg.print();
    return true;
}

bool Foo::handle(const MsgPosition& msg, void* userData) const
{ 
    msg.print();
    return true;
}


//////////////////////////////////////////////////

void MessageBuffer::append(std::string str)
{
    buffer += str;
}

std::string MessageBuffer::getMessage()
{
    int buflen = buffer.length();
    if (buflen < 4)
	throw MessageNotReady();

    int msglen = atoi(buffer.substr(0, 4).c_str());
    if (msglen > buflen)
	throw MessageNotReady();

    buffer.erase(0, 5);
    msglen -= 5;
    std::string message = buffer.substr(0, msglen);
    buffer.erase(0, msglen);
    return message;
}

//////////////////////////////////////////////////

#if 0
int main()
{
    Foo foo;

    MessageBuffer mb;

    std::ostringstream ostr;

    MsgJoinReq msgJoinReq("foo", 1234);
    msgJoinReq.encode(ostr);

    MsgJoinAccept msgJoinAccept(4321);
    msgJoinAccept.encode(ostr);

    MsgJoinReject msgJoinReject;
    msgJoinReject.encode(ostr);

    mb.append(ostr.str());
    std::cout << ostr.str() << std::endl;

    try 
    {
	while (true)
	{
	    std::string message = mb.getMessage();
	    std::cout << "Message: " << message << std::endl;
	    std::istringstream istr(message);
	    Message* msg = Message::decode(istr);
	    msg->handle(&foo, 0);
	}
    }
    catch (MessageNotReady& ex)
    {
	std::cout << "Message not ready" << std::endl;
    }

    return 0;
}
#endif


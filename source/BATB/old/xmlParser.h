#ifndef _XMLPARSER_H
#define _XMLPARSER_H

#include "message.h"

#if 0
#define FREEBSD
#endif

namespace batb
{

typedef enum 
{
    // tag             example
    //---------------------------------------
    TT_xmlBeginTag, // <foo>
    TT_xmlEndTag,   // </foo>
    TT_integer,     // 123
    TT_real,        // 123.321
    TT_boolean,     // true/false
    TT_string,      // Jeeves
    TT_eof,         // end of file/string
    TT_error        // error in the stream
} TokenType;


struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType t, std::string v) : type(t), value(v) {}

    ~Token() {}

    bool match(TokenType type, const char* value)
    {
	return (this->type == type) && (this->value == value);
    }

    bool match(TokenType type)
    {
	return (this->type == type);
    }
};

class TokenStream : public std::istream
{
public:
    TokenStream(std::streambuf* buffer);
    TokenStream& operator>>(Token& token);
    bool parse(char* xmltag, char** value);
    int line() { return lineNumber; }
    int get();
    void error(std::string msg);

private:
    int lineNumber;
    int column;
    std::string currentLine;

};


class XmlParser
{
 public:
    void parse(TokenStream& input, Database& db) const;
    void readFile(const std::string& filename, Database& db) const;
    bool msgEventInfoXMLparse(std::istream& istr, MsgEventInfo*& msgEventInfo) const;
#if 0
    bool msgRunParse(std::istream& istr, MsgRun*& msgRun) const;
#endif

    // implements the singleton pattern
    static XmlParser& instance()
    {
	static XmlParser theSingleInstance;
	return theSingleInstance;
    }

    void readRoute(Run* run) const;

 private:
    bool parse(TokenStream& input, TokenType type, const char* xmlTag, Token& token) const;

    bool parse(TokenStream& input, const char* xmlTag, std::string& s) const;
    bool parse(TokenStream& input, const char* xmlTag, fptype& f) const;
    bool parse(TokenStream& input, const char* xmlTag, int& i) const;
#ifndef FREEBSD
    bool parse(TokenStream& input, const char* xmlTag, time_t& t) const;
#endif
    bool parse(TokenStream& input, const char* xmlTag, bool& b) const;
    bool parse(TokenStream& input, const char* xmlTag, Run::RunStatus& r) const;

    bool parse(TokenStream& input, SplitTime*& st) const;
    bool parse(TokenStream& input, Terrain*& terrain) const;
    bool parse(TokenStream& input, Map*& map) const;
    bool parse(TokenStream& input, Point*& point) const;
    bool parse(TokenStream& input, Course*& course) const;
    bool parse(TokenStream& input, Orienteer*& orienteer) const;
    bool parse(TokenStream& input, Event*& event) const;
    bool parseRoute(TokenStream& input, Run*& run) const;
    bool parse(TokenStream& input, Run*& run) const;
    bool parse(TokenStream& input, ServerInfo*& serverInfo) const;

    void parseTerrains(TokenStream& input, Database& db) const;
    void parseMaps(TokenStream& input, Database& db) const;
    void parseCourses(TokenStream& input, Database& db) const;
    void parseEvents(TokenStream& input, Database& db) const;
    void parseOrienteers(TokenStream& input, Database& db) const;
    void parseRuns(TokenStream& input, Database& db) const;
    void parseServers(TokenStream& input, Database& db) const;

    XmlParser() {}

};

}

#endif // _XMLPARSER_H 

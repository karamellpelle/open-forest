//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#ifndef BATB_RUN_CONSOLE_HPP
#define BATB_RUN_CONSOLE_HPP
#include "batb/batb_include.hpp"


namespace batb
{
class BATB;

namespace run
{
class World;
class Console;
class TBConsole;


// http://www.angelikalanger.com/Articles/C++Report/IOStreamsDerivation/IOStreamsDerivation.html
class ConsoleStreambuf : public std::streambuf
{
public:
    ConsoleStreambuf(Console& c) : console_( c )    { }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override;
    virtual int overflow (int c) override;

private:
    Console& console_;

};


class Console : public std::ostream
{
friend class ConsoleStreambuf;

public:
    Console(BATB* b) : std::ostream( &streambuf_ ), batb( b ), streambuf_( *this ) { }

    void begin();
    void end();

    void step(World& );

    // show and hide
    void open(World& );
    void close(World& );

    std::string getPS1();

    // command to console
    bool cmd(const std::string& );
    bool operator()(const std::string& s) { return this->cmd( s ); }

    BATB* batb;


    TBConsole* tb_console = nullptr;

private:
    ConsoleStreambuf streambuf_;
    std::string ps1_;

};


template <typename T>
std::ostream& operator<<(std::unique_ptr<Console>& l, const T& t)
{
    return l->operator<<( t );
}
inline std::ostream& operator<<(std::unique_ptr<Console>& l, const std::string& str)
{
    return operator<<( *l, str );
}


////////////////////////////////////////////////////////////////////////////////


} // namespace run

} // namespace batb

#endif

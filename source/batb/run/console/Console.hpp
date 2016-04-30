//    open-forest: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
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
friend void begin(Console& );
friend void end(Console& );
friend class ConsoleStreambuf;

public:
    Console(BATB& b) : std::ostream( &streambuf_ ), batb( b ), streambuf_( *this ) { }

    void step(World& );

    // show and hide
    void open(World& );
    void close(World& );

    std::string getPS1();

    // command to console
    bool operator()(const std::string& );

    BATB& batb;


    TBConsole* tb_console = nullptr;

private:
    ConsoleStreambuf streambuf_;
    std::string ps1_;

};


////////////////////////////////////////////////////////////////////////////////

void begin(Console& );

void end(Console& );

////////////////////////////////////////////////////////////////////////////////


} // namespace run

} // namespace batb

#endif

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
#ifndef BATB_LOG_HPP
#define BATB_LOG_HPP
#include "include.hpp"

namespace batb
{


// defining custom std::ostream's:
// http://www.angelikalanger.com/Articles/C++Report/IOStreamsDerivation/IOStreamsDerivation.html


class Log : public std::ostream, public std::streambuf 
{
public:
    Log();


    virtual std::streamsize xsputn(const char* s, std::streamsize n) override;
    virtual int overflow (int c) override;

    //std::ostream& endl();
    //std::ostream& flush();

    // TODO: tag messages
    //std::ostream& info();
    //std::ostream& error();
    //

    // TMP:
    //std::ostream& operator<<(std::ostream&) { return std::cout; }

private:

    // TODO:
    //std::ostream* os_; // from definition.
    //uint line_indentation_ = 0;
};





} // namespace batb

#endif

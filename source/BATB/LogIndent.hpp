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
#ifndef BATB_LOGINDENT_HPP
#define BATB_LOGINDENT_HPP
#include "BATB/Log.hpp"

namespace batb
{


////////////////////////////////////////////////////////////////////////////////
// LogIndent : indentation made simple!

class LogIndent
{
public:
    // indentation
    LogIndent(const std::unique_ptr<Log>& );
    // indentation + width
    LogIndent(const std::unique_ptr<Log>& , uint );
    // indentation + prefix
    LogIndent(const std::unique_ptr<Log>& , const std::string& );
    // indentation + width + prefix
    LogIndent(const std::unique_ptr<Log>& , uint , const std::string& );

    // destructor
    ~LogIndent();

private:
    Log* log_ = nullptr;
    std::string prefix_prev_;
};


////////////////////////////////////////////////////////////////////////////////

// indentation
inline LogIndent::LogIndent(const std::unique_ptr<Log>& l) : LogIndent( l, Log::default_indent_wth, "" )
{

}

// indentation (custom width)
inline LogIndent::LogIndent(const std::unique_ptr<Log>& l, uint w) : LogIndent( l, w, "" )
{

}

// indentation + prefix
inline LogIndent::LogIndent(const std::unique_ptr<Log>& l, const std::string& pre) : LogIndent( l, Log::default_indent_wth, pre )
{

}


// indentation (custom width) + prefix
inline LogIndent::LogIndent(const std::unique_ptr<Log>& l, uint w, const std::string& pre) : log_( l.get() )
{
    prefix_prev_ = log_->prefix();
    log_->indentWidth( w );
    log_->prefix( pre );
    log_->indentPush();
}


// destructor
inline LogIndent::~LogIndent()
{
    // back to previous indentation
    log_->indentPop();

    // set prefix back to previous value
    log_->prefix( prefix_prev_ );
}


} // namespace batb

#endif


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
#include "BATB/Log.hpp"

////////////////////////////////////////////////////////////////////////////////
//  


namespace batb
{

Log::Log() : std::ostream( this )
{

}

//std::ostream& Log::endl()
//{
//    return operator<<( std::endl );
//}
//
//std::ostream& Log::flush()
//{
//    return operator<<( std::endl );
//}


std::streamsize Log::xsputn(const char* s, std::streamsize n)
{
    std::string str;

    auto b = s;
    auto e = s + n;

//std::cout << "\nxsputn" << std::endl;

    while ( b != e )
    {
        // find newline character
        auto e1 = std::find( b, e, '\n' );

        if ( b == e1 )
        {
            // (we are standing on a newline character)

//std::cout << "  (newline)" << std::endl;

            // are we in the beginning of a newline?
            if ( newline_ )
            {
                // shall we ident empty lines?
                if ( indent_newline_ )
                {
                    // write indent
                    uint wth = std::accumulate( std::begin( indents_ ), std::end( indents_ ), 0 );
                    str += std::string( wth, ' ' );
                    
                    // shall we add the prefix too?
                    if ( indent_newline_prefix_ )
                    {
                        // write prefix
                        str += prefix_;
                    }
                }
            }

            // write the newline character
            str += '\n';

            // we are now in the beginning of a new line
            newline_ = true;

            // continue
            ++b;
        }
        else
        {
            // (the string [b, e1) is nonempty and does not contain newline)

//std::cout << "  '" << std::string(b, e1) << "'" << std::endl;
            
            // are we in the beginning of a new line?
            if ( newline_ )
            {
                // write indent + prefix
                uint wth = std::accumulate( std::begin( indents_ ), std::end( indents_ ), 0 );
                str += std::string( wth, ' ' ) + prefix_;
            }

            // write [b, e1)
            str += std::string( b, e1 );

            // now we are not in the beginning of a new line, since [b, e1)
            // was nonempty and did not contain newlines
            newline_ = false;

            // continue
            b = e1;
        }
            
    }

    // instead of using my_sputn(), do it easy:
    std::cout << str;
    return n;
}

int Log::overflow (int c)
{
    xsputn( (const char*)( &c ), 1 );
    return c;
}

void Log::indentPush()
{
    indents_.push_back( indent_wth_ );

}
void Log::indentPop()
{
    if ( !indents_.empty() ) indents_.pop_back();

    // set back indent
    indent_wth_ = indents_.empty() ? default_indent_wth : indents_.back();

}
void Log::indentWidth(uint n)
{
    indent_wth_ = n;
}
void Log::prefix(const std::string& pre)
{
    prefix_ = pre;
}
std::string Log::prefix() const
{
    return prefix_;
}

////////////////////////////////////////////////////////////////////////////////
//
std::streamsize Log::my_sputn(const char* s, std::streamsize n)
{
    // (this function ignored at the moment)
    //
    // write to std::cout, but this can later be constructed to
    // do file logging or change to std::cerr for errors.
    return std::cout.rdbuf()->sputn( s, n );
}


} // namespace batb

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
#include "batb.hpp"
#include "batb/run/Run.hpp"
#include "batb/run/Console.hpp"
#include "batb/run/Console/parse.hpp"



  
namespace batb
{

namespace run
{

void Console::open()
{
    //tb_console_->SetVisibilility( tb::WIDGET_VISIBILITY_VISIBLE );
    batb.log << "TODO: console open" << std::endl;
}

void Console::close()
{
    //tb_console->SetVisibilility( tb::WIDGET_VISIBILITY_INVISIBLE );
    batb.log << "TODO: console close" << std::endl;
}

void Console::operator()(const std::string& input)
{
    // specific command handle functions.
    // these are allowed to modify input. 
    using CommandEater = void(Console& , std::string& );
    extern CommandEater command_echo;
    extern CommandEater command_value;

    std::string in = input;
    std::string cmd = word( in );

    if ( cmd == "echo" )
    {
        command_echo( *this, in );   
        return;
    }
    if ( cmd == "value" )
    {
        command_value( *this, in );
        return;
    }

    batb.log << "run::Console: command not found: " << cmd 
             << std::endl;
  
}



////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


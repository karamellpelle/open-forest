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
#include "batb/batb_include.hpp"
#include "tb/tb_system.h"

#include <stdio.h>

namespace tb
{


// FIXME: 
//
//  * this is currently only a copy of TBPosixFile form turbobadger.
//    look at tb_system_win/android for implementation for others.
//    hopefully use <ifstream>, <cstdio>, etc. can be used for all
//    platforms!
//
class TBFileBATB : public TBFile
{
public:
	TBFileBATB(FILE *f) : file(f) {}
	virtual ~TBFileBATB() { fclose(file); }

	virtual long Size()
	{
		long oldpos = ftell(file);
		fseek(file, 0, SEEK_END);
		long num_bytes = ftell(file);
		fseek(file, oldpos, SEEK_SET);
		return num_bytes;
	}
	virtual size_t Read(void *buf, size_t elemSize, size_t count)
	{
		return fread(buf, elemSize, count, file);
	}

        static TBFile* OpenImpl(const char* filename, TBFileMode mode);
private:
	FILE *file;


};



TBFile* TBFileBATB::OpenImpl(const char* filename, TBFileMode mode)
{
	FILE *f = nullptr;
	switch (mode)
	{
	case TBFile::MODE_READ:
		f = fopen(filename, "rb");
		break;
	default:
		break;
	}
	if (!f)
		return nullptr;
	TBFileBATB *tbf = new TBFileBATB(f);
	if (!tbf)
		fclose(f);
	return tbf;
}



const char* strip_protocol(const char* ref, const char* b)
{
    while ( *ref != '\0' )
    {
        if ( *ref != *b )   return nullptr; // this also handles the case when b empty (i.e. *b == '\0')

        ++ref;
        ++b;
    }

    return b;
}

// this is used by turbobadger to retrieve given filenames.
// we allow "protocol specifiers" to define where to look.
TBFile *TBFile::Open(const char *filename, TBFileMode mode)
{
        // define search path:
        //  static://   : file::static_data()
        //  dynamic://  : file::dynamic_data()
        //  user://     : file::user_data()
        //              : 'filename'

        if ( auto path = strip_protocol( "static://"  , filename ) ) return TBFileBATB::OpenImpl( file::static_data( path ).c_str(),  mode );
        if ( auto path = strip_protocol( "dynamic://" , filename ) ) return TBFileBATB::OpenImpl( file::dynamic_data( path ).c_str(), mode );
        if ( auto path = strip_protocol( "user://"    , filename ) ) return TBFileBATB::OpenImpl( file::user( path ).c_str(),         mode );
        
        // if no protocol specified, use given path
        return TBFileBATB::OpenImpl( filename , mode );
}



} // namespace tb


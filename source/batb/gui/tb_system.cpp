//    orienteering-game: an orientering game.
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

#include "tb/tb_core.h"
#include "tb/renderers/tb_renderer_gl.h"
#include "tb/tb_font_renderer.h"
#include "tb/animation/tb_animation.h"
#include "tb/animation/tb_widget_animation.h"
#include "tb/tb_system.h"

#include <stdio.h>


// interface for batb::gui to 
namespace batb
{

namespace gui
{

static double tbsystem_time = 0.0;



// set time for TBSystem:
void tbsystem_ms(double t)
{
    tbsystem_time = t * 1000;   

}

// TODO: inteface for DPI, ...


} // namespace gui

} // namespace batb


namespace tb
{

////////////////////////////////////////////////////////////////////////////////
//  TBSystem


// this function is called by TBMessageHandler when the time for the 
// next message to be delivered got changed. however, we do not use
// such wake-up system in our game application, instead a continuous
// loop, so we do not care about this notification.
void TBSystem::RescheduleTimer(double fire_time)
{

}



double TBSystem::GetTimeMS()
{
/*
	struct timeval now;
	gettimeofday( &now, NULL );
	return now.tv_usec/1000 + now.tv_sec*1000;
*/
    return batb::gui::tbsystem_time;
}


// only for touch platforms
int TBSystem::GetLongClickDelayMS()
{
	return 500;
}

// only for touch platforms
int TBSystem::GetPanThreshold()
{
    // FIXME!
    // (probably not necessary, only for touch platforms...)
	return 5 * GetDPI() / 96;
}

// only for touch platforms
int TBSystem::GetPixelsPerLine()
{
	return 40 * GetDPI() / 96;
}

// we currently use this during our GUI setup for fonts
// FIXME: make our TB implementation independent of this,
//        so the view across different screens is mostly consistent. 
//        we are also using MSAA typically, anyway
int TBSystem::GetDPI()
{
	// FIX: Implement!
	return 96;
}


////////////////////////////////////////////////////////////////////////////////
//  TBFile


// this implementation seems to be boundn to standard C library, 
// not Linux...
class TBLinuxFile : public TBFile
{
public:
	TBLinuxFile(FILE *f) : file(f) {}
	virtual ~TBLinuxFile() { fclose(file); }

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
private:
	FILE *file;
};

TBFile* TBFile::Open(const char *filename, TBFileMode mode)
{
	FILE *f = nullptr;
	switch (mode)
	{
	case MODE_READ:
		f = fopen(filename, "rb");
		break;
	default:
		break;
	}
	if (!f)
		return nullptr;
	TBLinuxFile *tbf = new TBLinuxFile(f);
	if (!tbf)
		fclose(f);
	return tbf;
}

////////////////////////////////////////////////////////////////////////////////
//  clipboard (empty)
//
static TBStr clipboard; ///< Obviosly not a full implementation since it ignores the OS :)

void TBClipboard::Empty()
{
	clipboard.Clear();
}

bool TBClipboard::HasText()
{
	return !clipboard.IsEmpty();
}

bool TBClipboard::SetText(const char *text)
{
	return clipboard.Set(text);
}

bool TBClipboard::GetText(TBStr &text)
{
	return text.Set(clipboard);
}

} // namespace tb




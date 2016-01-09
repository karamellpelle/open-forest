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
#include "batb/demo/libs/al.hpp"
#include "batb.hpp"
#include "batb/al.hpp"
#include "batb/run/World.hpp"


namespace batb
{


namespace demo
{

namespace al
{



static bool tmp_empty = true;

static alure::SharedPtr<alure::Decoder> decoder = nullptr;
static alure::Source* source = nullptr;
static bool once = false;


void demo_begin(BATB& batb)
{

    if ( tmp_empty )
    {

    }
    tmp_empty = false;
}

void demo_play(BATB& batb, const std::string& path)
{
    decoder = batb.al.alure_context->createDecoder( path );
    if ( decoder == nullptr ) return;

    source = batb.al.alure_context->getSource();
    source->play( decoder, 32768, 4);
/*
    std::cout << "demo::al: " << path 
              << " ("
              << alure::GetSampleTypeName(decoder->getSampleType())<< ", "
              << alure::GetSampleConfigName(decoder->getSampleConfig()) 
              << ", "
              << decoder->getFrequency() << "hz"
              << ")" 
              << std::endl;
*/
}

void demo_iterate(BATB& batb, run::World& world)
{
    // TODO: create media player with nanovg
/*
    if ( source->isPlaying() )
    {
        // ...
        // TODO: modify source (effects, position, ...)
       
        // TODO: write to screen with nanovg:
        float invfreq = 1.0f / decoder->getFrequency();
        float off_sec = source->getOffset() * invfreq;
        float len_sec = decoder->getLength() * invfreq;
    }
    else
    {
        if ( !once )
        {
            std::cout <<  "demo::al: source stopped." << std::endl;
            once = true;
        }

        // TODO: load new
    }
*/
}


void demo_end(BATB& batb)
{
    source->release();
    source = nullptr;
    decoder.reset();

}



} // namespace al

} // namespace demo


} // namespace batb





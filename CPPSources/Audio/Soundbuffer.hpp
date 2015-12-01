#pragma once

#include <string>
#include "Shortcuts.h"
#include "vorbisfile.h"

struct Soundbuffer{
    string     name;
    OggVorbis_File  *file;
    vorbis_info     *info;
    unsigned int    bid[4];
    
    
    bool load(const string fileName);
    
    
};
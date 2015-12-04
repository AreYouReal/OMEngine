#pragma once

#include "OMUtils.h"
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

class Sound{
public:
    
    Sound(const std::string name, Soundbuffer *sBuffer);
    ~Sound();
    
    void play(int loop);
    void pause();
    void stop();
    void rewind();
    void updateQueue();
    
    float getTime();
    int   getState();
    float getVolume();
    
    void setSpeed(float speed);
    void setVolume(float volume);
    void setLocation(const v3d loc, float refDistance);
    
private:
    std::string name;
    unsigned int sid;
    int loop;
    Soundbuffer *sBuffer;
    
    void free();
};


#pragma once

#include "Singleton.hpp"
#include "vorbisfile.h"

class Boombox : public Singleton<Boombox>{
public:
    Boombox();
    virtual ~Boombox();
    
    
    void play();
    
    ov_callbacks callbacks;
    
    
    void checkObbFunctionality();
    
private:
    

    
    void createAndLoadSoundBuffer();
    
    
    ALCdevice   *device;
    ALCcontext  *context;
    
    ALuint  sbuffer;
    ALuint  sSource;
    
    
    size_t oggRead(void *ptr, size_t size, size_t read, void *memoryPtr);
    int     oggSeek(void *memoryPtr, ogg_int64_t offset, int stride);
    long    oggTell(void *memoryPtr);
    int     oggClose(void *memoryPtr);
};
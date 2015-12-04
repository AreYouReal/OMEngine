#pragma once

#include "Singleton.hpp"
#include "vorbisfile.h"
#include "Sound.hpp"

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
    
    
    static size_t  oggRead(void *ptr, size_t size, size_t read, void *memoryPtr);
    static int     oggSeek(void *memoryPtr, ogg_int64_t offset, int stride);
    static long    oggTell(void *memoryPtr);
    static int     oggClose(void *memoryPtr);
    
    
    Soundbuffer *oggSoundBuffer;
    Sound   *oggTestSound;
    
    
    void error();
};
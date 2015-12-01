#pragma once

#include "Singleton.hpp"

class Boombox : public Singleton<Boombox>{
public:
    Boombox();
    virtual ~Boombox();
    
    
    void play();
    
private:
    
    
    void createAndLoadSoundBuffer();
    
    
    ALCdevice   *device;
    ALCcontext  *context;
    
    ALuint  sbuffer;
    ALuint  sSource;
    
};
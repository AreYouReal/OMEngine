#pragma once

#include "Singleton.hpp"

class Boombox : public Singleton<Boombox>{
public:
    Boombox();
    virtual ~Boombox();
    
private:
    
//    ALCDevice   *device;
//    ALCcontext  *contex;
    
};
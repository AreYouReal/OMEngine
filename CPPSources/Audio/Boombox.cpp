#include "Boombox.hpp"


#include "OMUtils.h"
#include "Shortcuts.h"
#include "Game.h"


Boombox::Boombox(){
    logMessage("Boombox constructor!\n");
    
    device = alcOpenDevice(nullptr);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    
    
    createAndLoadSoundBuffer();
}


Boombox::~Boombox(){
    
    alDeleteBuffers(1, &sbuffer);
    alDeleteSources(1, &sSource);
    
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    logMessage("Boombox destructor!\n");
}


void Boombox::play(){
    alSourcePlay(sSource);
}

void Boombox::createAndLoadSoundBuffer(){
    alGenBuffers(1, &sbuffer);
    up<FileContent> soundSource = readBytesFromFile(Game::getAppContext(), "test.raw");
    
    if(!soundSource) return;
    
    
    alBufferData(sbuffer, AL_FORMAT_MONO16, soundSource->content, soundSource->size, 22050);
    
    
    alGenSources(1, &sSource);
    alSourcei(sSource, AL_BUFFER, sbuffer);
}

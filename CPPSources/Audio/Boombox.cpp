#include "Boombox.hpp"


#include "OMUtils.h"
#include "Shortcuts.h"
#include "Game.h"


#include "Soundbuffer.hpp"

Boombox::Boombox(){
    logMessage("Boombox constructor!\n");
    
    device = alcOpenDevice(nullptr);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    
    logMessage( "\nAL_VENDOR:     %s\n", ( char * )alGetString ( AL_VENDOR     ) );
    logMessage( "AL_RENDERER:     %s\n"  , ( char * )alGetString ( AL_RENDERER   ) );
    logMessage( "AL_VERSION:      %s\n"  , ( char * )alGetString ( AL_VERSION    ) );
    logMessage( "AL_EXTENSIONS:   %s\n"  , ( char * )alGetString ( AL_EXTENSIONS ) );
//    
//    audio.callbacks.read_func  = AUDIO_ogg_read;
//    audio.callbacks.seek_func  = AUDIO_ogg_seek;
//    audio.callbacks.tell_func  = AUDIO_ogg_tell;
//    audio.callbacks.close_func = AUDIO_ogg_close;
    
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

void Boombox::checkObbFunctionality(){
    Soundbuffer sb;
    if(sb.load("lounge.ogg")){
    
    }else{
    
    }

}




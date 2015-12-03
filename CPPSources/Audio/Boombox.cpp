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
    
    logMessage( "\nAL_VENDOR:     %s\n",   ( char * )alGetString ( AL_VENDOR     ) );
    logMessage( "AL_RENDERER:     %s\n"  , ( char * )alGetString ( AL_RENDERER   ) );
    logMessage( "AL_VERSION:      %s\n"  , ( char * )alGetString ( AL_VERSION    ) );
    logMessage( "AL_EXTENSIONS:   %s\n"  , ( char * )alGetString ( AL_EXTENSIONS ) );
    
    callbacks.read_func     = Boombox::oggRead;
    callbacks.seek_func     = Boombox::oggSeek;
    callbacks.tell_func     = Boombox::oggTell;
    callbacks.close_func    = Boombox::oggClose;
    
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
    if(sb.load("00.ogg")){
    
    }else{
    
    }

}

size_t Boombox::oggRead(void *ptr, size_t size, size_t read, void *memoryPtr){
    unsigned int seof, pos;
    FileContent *contentPointer = (FileContent*)memoryPtr;
    
    seof = contentPointer->size;
    
    pos = (read * size) < seof ? read * size : seof;
    
    
    if(pos){
        memcpy(ptr, contentPointer->content, pos);
    }
    
    return pos;
}

int Boombox::oggSeek(void *memoryPtr, ogg_int64_t offset, int stride){
    return -1;
}


long    Boombox::oggTell(void *memoryPtr){
    return -1;
}


int     Boombox::oggClose(void *memoryPtr){
    return -1;
}

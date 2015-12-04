#include "Boombox.hpp"


#include "OMUtils.h"
#include "Shortcuts.h"
#include "Game.h"




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
    
            oggTestSound->play(1);
//    alSourcePlay(sSource);
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
    oggSoundBuffer = new Soundbuffer();
    if(oggSoundBuffer->load("lounge.ogg")){
        oggTestSound = new Sound("lounge.ogg", oggSoundBuffer);
    }else{
    
    }

}

size_t Boombox::oggRead(void *ptr, size_t size, size_t read, void *memoryPtr){
    unsigned int seof, pos;
    FileContent *contentPointer = (FileContent*)memoryPtr;
    
    seof = contentPointer->size - contentPointer->position;
    
    pos = ((read * size) < seof) ? read * size : seof;
    
    
    if(pos){
        memcpy(ptr, contentPointer->content + contentPointer->position, pos);
        contentPointer->position += pos;
    }
    logMessage("%d, %lu, %lu\t", contentPointer->position, size, read);
    return pos;
}

int Boombox::oggSeek(void *memoryPtr, ogg_int64_t offset, int stride){
    unsigned int pos;
    
    FileContent *cPtr = (FileContent*)memoryPtr;

    switch (stride) {
        case SEEK_SET:{
            pos = (cPtr->size >= offset) ? (unsigned int)offset : cPtr->size;
            cPtr->position = pos;
            break;
        }
        case SEEK_CUR:{
            unsigned int seof = cPtr->size - cPtr->position;
            pos = (offset < seof) ? (unsigned int)offset : seof;
            break;
        }
        case SEEK_END:{
            cPtr->position = cPtr->size + 1;
            break;
        }
    }
    
    
    return 0;
}


long    Boombox::oggTell(void *memoryPtr){
    FileContent *fc =(FileContent*)memoryPtr;
    return fc->position;
}


int     Boombox::oggClose(void *memoryPtr){
    return memoryPtr ? 1 : 0;
}

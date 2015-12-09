#include "Sound.hpp"

#include "FileContent.h"
#include "Game.h"
#include "Boombox.hpp"


#pragma mark Constructor/destructor

Sound::Sound(const std::string name, Soundbuffer *sBuffer){
    this->name = name;
    this->sBuffer = sBuffer;
    
    alGenSources(1, &sid);
    
    alSource3f(sid, AL_POSITION, 0, 0, 0);
    alSource3f(sid, AL_VELOCITY, 0, 0, 0);
    alSource3f(sid, AL_DIRECTION, 0, 0, 0);
    
    alSourcef(sid, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcei(sid, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef(sid, AL_GAIN, 1.0f);
    alSourcef(sid, AL_PITCH, 1.0f);
    
    logMessage("Sound constructor!\n");
}


Sound::~Sound(){
    free();
    logMessage("Sound destructor!\n");
}

#pragma mark Public
void Sound::play(int loop){
//    alSourcef(sid, AL_GAIN, 1.0f);
    this->loop = loop;
    if(!sBuffer->bid[1]){
        alSourcei(sid, AL_LOOPING, loop);
        alSourcei(sid, AL_BUFFER, sBuffer->bid[0]);
    }else{
        alSourceQueueBuffers(sid, 4, &sBuffer->bid[0]);
    }
    alSourcePlay(sid);
}

void Sound::pause(){
    alSourcePause(sid);
}

void Sound::stop(){
    alSourceStop(sid);
}

void Sound::setSpeed(float speed){
    alSourcef(sid, AL_PITCH, speed);
}

void Sound::setVolume(float volume){
    alSourcef(sid, AL_GAIN, volume);
}

void Sound::setLocation(const v3d loc, float refDistance){
    alSourcei(sid, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcef(sid, AL_REFERENCE_DISTANCE, refDistance);
    alSource3f(sid, AL_POSITION, loc.x, loc.y, loc.z);
}

int Sound::getState(){
    int state = 0;
    alGetSourcei(sid, AL_SOURCE_STATE, &state);
    return state;
}

void Sound::rewind(){
    alSourceRewind(sid);
}

float Sound::getTime(){
    float playbackTime = 0.0f;
    alGetSourcef(sid, AL_SEC_OFFSET, &playbackTime);
    return playbackTime;
}

float Sound::getVolume(){
    float volume = 0.0f;
    alGetSourcef(sid, AL_GAIN, &volume);
    return volume;
}

void Sound::updateQueue(){
//    unsigned int i = 0;
//    
//    int p,
//    q;
//    
//    alGetSourcei( sound->sid, AL_BUFFERS_PROCESSED, &p );
//    
//    alGetSourcei( sound->sid, AL_BUFFERS_QUEUED, &q );
//    
//    while( p-- )
//    {
//        unsigned int bid;
//        
//        alSourceUnqueueBuffers( sound->sid,
//                               1,
//                               &bid );
//        
//        while( i != MAX_BUFFER )
//        {
//            if( bid == sound->soundbuffer->bid[ i ] ) break;
//            
//            ++i;
//        }
//        
//        if( SOUNDBUFFER_decompress_chunk( sound->soundbuffer, i ) )
//        {
//            alSourceQueueBuffers( sound->sid,
//                                 1,
//                                 &bid );
//        }
//    }
//    
//    
//    if( !q && sound->loop )
//    {
//        AUDIO_ogg_seek( sound->soundbuffer->file->datasource,
//                       0,
//                       SEEK_SET );
//        
//        while( i != MAX_BUFFER )
//        {
//            SOUNDBUFFER_decompress_chunk( sound->soundbuffer, i );
//            ++i;
//        }
//        
//        SOUND_play( sound, sound->loop );
//    }
}

#pragma mark Helpers
void Sound::free(){
    if(sid){
        stop();
        alDeleteSources(1, &sid);
    }
}



bool Soundbuffer::load(const std::string fileName){
    name = fileName;
    file = new OggVorbis_File();
    
    up<FileContent> fileSource = readBytesFromFile(Game::getAppContext(), fileName.c_str());
    
    if(!fileSource){ return false; }
    
    ov_open_callbacks(fileSource.get(), file, nullptr, 0, Boombox::instance()->callbacks);
    
    info = ov_info(file, -1);
    
    if(info){
        unsigned int size = ((unsigned int)ov_pcm_total(file, -1) * info->channels << 1);
        
        int count, bit;
        
        char *data = (char*) malloc(size), *start = data;
        
        
        unsigned int maxChunkSize = 1024 << 3;
        while((count = ov_read(file, start, maxChunkSize, 0, 2, 1, &bit)) > 0){
            start += count;
        }
        
        alGenBuffers(1, &bid[0]);
        alBufferData(bid[0], info->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, size, info->rate);
        
        free(data);
    }
    
    ov_clear(file);
    delete file;
    file = nullptr;
    info = nullptr;
    return true;
}

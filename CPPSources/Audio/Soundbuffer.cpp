#include "Soundbuffer.hpp"

#include "OMUtils.h"
#include "FileContent.h"
#include "Game.h"
#include "Boombox.hpp"

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
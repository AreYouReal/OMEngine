#pragma once

#include "Shortcuts.h"

#include "OMUtils.h"

template<typename T>
class Singleton{
public:

    virtual ~Singleton(){}

    static T* instance(){
        if (!mInstance) {
            mInstance = new T();
        }
        return mInstance;
    }
    
    static void destroy(){
        if(mInstance){
            delete mInstance;
            mInstance = nullptr;
        }
    }
    
protected:
    Singleton(){}
    static T* mInstance;
    
};

template<typename T>
T* Singleton<T>::mInstance;
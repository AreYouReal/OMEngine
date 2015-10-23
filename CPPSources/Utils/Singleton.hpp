#pragma once

#include "Shortcuts.h"

template<typename T>
class Singleton{
public:

    virtual ~Singleton(){
        if(mInstance){
            delete mInstance;
            mInstance = nullptr;
        }
    }

    static T* instance(){
        if (!mInstance) {
            mInstance = new T();
        }
        return mInstance;
    }
    
protected:
    Singleton(){}
    static T* mInstance;
    
};

template<typename T>
T* Singleton<T>::mInstance;
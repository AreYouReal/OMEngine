#pragma once

#include "Shortcuts.h"

template<typename T>
class Singleton{
public:

    virtual ~Singleton(){}

    static sp<T> instance(){
        auto retVal = mInstance.lock();
        if (!retVal) {
            retVal = std::make_shared<T>();
            mInstance = retVal;
        }
        return retVal;
    }
    
protected:
    Singleton(){}
    static wp<T> mInstance;
    
};

template<typename T>
wp<T> Singleton<T>::mInstance;
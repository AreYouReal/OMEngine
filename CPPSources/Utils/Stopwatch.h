#pragma once
#include <ctime>
#include "OMUtils.h"

struct Stopwatch{
    Stopwatch(){ start = std::clock(); }
    ~Stopwatch(){ }
    float elapsedTime(){ return float(std::clock() - start) / CLOCKS_PER_SEC; }
    float fps(){ return 1.0f / elapsedTime(); }
    
private:
    clock_t start;
};
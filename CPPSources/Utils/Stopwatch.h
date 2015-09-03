#pragma once
#include <ctime>

struct Stopwatch{
    Stopwatch(){ start = std::clock(); }
    ~Stopwatch(){ }
    double elapsedTime(){ return double(std::clock() - start) / CLOCKS_PER_SEC; }
    
private:
    clock_t start;
};
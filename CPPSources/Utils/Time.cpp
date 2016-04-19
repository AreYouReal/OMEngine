#include "Time.hpp"
#include <ctime>


float Time::deltaTime;
float Time::scaleCoef = 1.0f;
float Time::startTime = time(NULL);


float Time::scaledTime(){
    return deltaTime * scaleCoef;
}

float Time::secondsSinceStart(){
   return difftime(startTime, time(NULL));
}
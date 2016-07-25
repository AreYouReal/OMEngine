#include <ctime>

struct Time{
    static float scaledTime();
    static float secondsSinceStart();
    
    static float deltaTime;
    static float scaleCoef;
    static float startTime;
};
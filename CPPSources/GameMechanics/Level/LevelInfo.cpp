#include "LevelInfo.hpp"
#include "OMUtils.h"
#include <sstream>


static v3d frontDir{0, 0, 1};
static v3d leftDir{-1, 0, 0};
static v3d backDir{0, 0, -1};
static v3d rightDir{1, 0, 0};
static int runStep{2};
static int jumpStep{8};

sp<LevelInfo> LevelInfo::parseLevelLine(const string line){
    if(line != ""){
        sp<LevelInfo> info = std::make_shared<LevelInfo>();
        
        v4d bColor;
        v4d bgColor;
        
        if(sscanf(line.c_str(), "%f %f %f %f %f %f %f %f %f %f %f ",
                  &info->camPos.x, &info->camPos.y, &info->camPos.z,
                  &bColor.x, &bColor.y, &bColor.z, &bColor.w,
                  &bgColor.x, &bgColor.y, &bgColor.z, &bgColor.w) == 11){
          
            
            info->bblockColor = bColor / 255.0f;
            info->bgColor = bgColor / 255.0f;
            
            std::string route = line.substr(line.find("f"));
            
            std::stringstream routeStream(route);
            
            vec<string> routeCommands;
            string word;
            while(getline(routeStream, word, ',')){
                routeCommands.push_back(word);
            }
            v3d prevPos(0, 0, -2);
            for(auto const &str : routeCommands){
                v3d currentDirVec;
                int step;
                if(!str.compare("f")){
                    currentDirVec = frontDir;
                    step = runStep;
                }else if(!str.compare("r")){
                    currentDirVec = rightDir;
                    step = runStep;
                }else if(!str.compare("b")){
                    currentDirVec = backDir;
                    step = runStep;
                }else if(!str.compare("l")){
                    currentDirVec = leftDir;
                    step = runStep;
                }else if(!str.compare("fj")){
                    currentDirVec = frontDir;
                    step = jumpStep;
                }else if(!str.compare("rj")){
                    currentDirVec = rightDir;
                    step = jumpStep;
                }else if(!str.compare("bj")){
                    currentDirVec = backDir;
                    step = jumpStep;
                }else if(!str.compare("lg")){
                    currentDirVec = leftDir;
                    step = jumpStep;
                }else{
                    logMessage("Unknown route command! %s\n", str.c_str());
                    continue;
                }
            
                info->poss.push_back(currentDirVec * step + prevPos);
                prevPos = prevPos + currentDirVec * step;
            }
        }
        return info;
    }

    return nullptr;
}


vec<sp<LevelInfo>> LevelInfo::parseWorld(const string content){
    std::stringstream ss(content);
    string line;
    vec<sp<LevelInfo>> world;
    while(std::getline(ss, line)){
        sp<LevelInfo> info = parseLevelLine(line);
        if(info) world.push_back(info);
    }
    
    return world;
}
#pragma once

#include "m4d.h"
#include "Shortcuts.h"

struct LevelInfo{
    v3d camPos;
    v4d bgColor;
    v4d bblockColor;
    vec<v3d> poss;

    static sp<LevelInfo> parseLevelLine(const string line);
    static vec<sp<LevelInfo>> parseWorld(const string content);
};
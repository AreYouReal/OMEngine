#pragma once

#include "v3d.h"

struct Material{
    v4d ambientColor;
    v4d diffuseColor;
    v4d specularColor;
    int shininess;
};
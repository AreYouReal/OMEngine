#pragma once

#include <vector>
#include "esUtil.h"

struct Mesh{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> tCoords;
};
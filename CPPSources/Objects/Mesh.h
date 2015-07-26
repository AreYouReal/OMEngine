#pragma once

#include <vector>
#include "SRUtils.h"

struct Mesh{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> tCoords;
};
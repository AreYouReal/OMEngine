#pragma once

#include "esUtil.h"

class Main{
public:
    static int  esMain ( ESContext *esContext );
    static int  Init( ESContext *esContext );
    static void Draw( ESContext *esContext );
    static void Shutdown( ESContext *esContext );
};
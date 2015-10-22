#pragma once

#include "SRUtils.h"

class Game{
    
public:
    static int  Main       ( SRContext * );
    static int  Init       ( SRContext * );
    static void Update     ( SRContext *, float);
    static void Draw       ( SRContext * );
    static void Shutdown   ( SRContext * );
    static void Touch      ( SRContext *, int, int, int );
    static void Exit       ();

    static    SRContext*      getAppContext();

private:
    static void printGLInfo();
};
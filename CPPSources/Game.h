#pragma once

#include "SRUtils.h"

class Game{
    
public:
    static int  Main       ( SRContext * );
    static int  Init       ( SRContext * );
    static void Update     ( SRContext *, const float);
    static void Draw       ( SRContext * );
    static void Shutdown   ( SRContext * );
    static void Touch      ( SRContext *, const int, const int, const int );
    static void Exit       ();

    static    SRContext*      getAppContext();
    
    static  bool debugFlag;

private:
    static void printGLInfo();
};
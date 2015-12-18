#pragma once

#include "OMUtils.h"

class OMGame{
    
public:
    static int  Main       ( OMContext * );
    static int  Init       ( OMContext * );
    static void Update     ( OMContext *, const float);
    static void Draw       ( OMContext * );
    static void Shutdown   ( OMContext * );
    static void Touch      ( OMContext *, const int, const int, const int );
    static void Exit       ();

    static    OMContext*      getAppContext();
    
    static  bool debugFlag;

private:
    static void printGLInfo();
    static void initOGL(const float width, const float height);
};
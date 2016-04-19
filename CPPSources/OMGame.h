#pragma once

#include "OMUtils.h"

class OMGame{
    
public:
    static int  StartUp         ( OMContext * );
    static int  InitGameWorld   ();
    static void Update          ( const float );
    static void Draw            ();
    static void Shutdown        ();
    static void Touch           ( const int, const int, const int );
    static void Exit            ( );

    static    OMContext*      getAppContext();
    static  bool debugFlag;

private:
    static void printGLInfo();
    static void initOGL(const float width, const float height);
};
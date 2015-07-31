#pragma once

#include "SRUtils.h"

struct SRGraphics{

    static int  Main       ( SRContext * );
    static int  Init       ( SRContext * );
    static void Draw       ( SRContext * );
    static void Shutdown   ( SRContext * );
    static void Touch      ( SRContext *, int, int, int );
    
    static    SRContext*      GetAppContext();
};
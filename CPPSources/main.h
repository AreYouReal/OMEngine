#pragma once

#include "SRUtils.h"

int  Main       ( SRContext * );
int  Init       ( SRContext * );
void Draw       ( SRContext * );
void Shutdown   ( SRContext * );
void Touch      ( SRContext *, int, int, int );
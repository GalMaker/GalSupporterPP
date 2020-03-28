#pragma once
/* ----------------------------------------------------------/
 * tools.h
 * ----------------------------------------------------------/
 * Utilities for frequently called conversions and others.
 * ---------------------------------------------------------*/

#include "settings.h"
#include "singleton.h"

class GS_Tools
{
private:
    GS_Tools(){}
    // DEFINE_SINGLETON(GS_Tools);
public:
    static SDL_Rect baselineToRaw(SDL_Rect src);
    static SDL_Point baselineToRaw(SDL_Point src);
    static SDL_Rect rawToBaseline(SDL_Rect src);
    static SDL_Point rawToBaseline(SDL_Point src);
};
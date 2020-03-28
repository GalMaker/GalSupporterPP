#pragma once
/* ----------------------------------------------------------/
 * events.h
 * ----------------------------------------------------------/
 * Event handler for Keyboard, mouse, game controllers.
 * ---------------------------------------------------------*/

#include <list>
#include <functional>
#include "basic_sdl.h"
#include "singleton.h"
#include "tools.h"

class GS_Events
{
    DEFINE_SINGLETON(GS_Events)
private:
    std::list<SDL_Event> events_;
public:
    // RangeFunc type == function(SDL_Point -> bool)
    // Notice: SDL_Point is in baseline
    using RangeFunc = std::function<bool(SDL_Point)>;

    // GS_Events::refresh()
    // Intended to be called once a main loop
    // Check events after called this function
    void refresh();

    // GS_Events::keyDown(code, clear=false)
    // Event Checker for specific key code
    // code: SDL_Scancode of key
    // clear: clear the event if TRUE
    bool keyDown(SDL_Scancode code, bool clear = false);

    // GS_Events::keyUp(code, clear=false)
    // Event Checker for specific key code
    // code: SDL_Scancode of key
    // clear: clear the event if TRUE
    bool keyUp(SDL_Scancode code, bool clear = false);

    // GS_Events::mouseLDown(code, clear=false)
    // Event Checker for mouse LEFT button down
    // range: function(baseline point -> bool)
    // clear: clear the event if TRUE
    bool mouseLDown(RangeFunc range, bool clear = false);

    // GS_Events::mouseLUp(code, clear=false)
    // Event Checker for mouse LEFT button up
    // range: function(baseline point -> bool)
    // clear: clear the event if TRUE
    bool mouseLUp(RangeFunc range, bool clear = false);

    // GS_Events::windowClose(clear=false)
    // Event Checker for window close(from window control)
    // clear: clear the event if TRUE
    bool windowClose(bool clear = false);
};
#pragma once
/* ----------------------------------------------------------/
 * basic_sdl.h
 * ----------------------------------------------------------/
 * Basic system based on SDL.
 * ---------------------------------------------------------*/

#include <map>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "singleton.h"
#include "settings.h"

struct GS_RenderInfo
{
    SDL_Texture*    tex;
    SDL_Rect        srcRect;
    SDL_Rect        dstRect;
    double          angle;
};

class GS_System
{
    DEFINE_SINGLETON(GS_System)
private:
    SDL_Window*     window_    = nullptr;
    SDL_Renderer*   renderer_  = nullptr;
    SDL_Texture*    texbuf_    = nullptr;
public:
    // Basic APIs
    GS_System();
    ~GS_System();
    SDL_Texture* imgLoad(const std::string& path);
    SDL_Texture* textLoad(const std::u8string& utf8string, TTF_Font* font, SDL_Color color, Uint32 width);
    SDL_Texture* surfaceLoad(SDL_Surface* sur);
    SDL_Texture* textureCreate(uint32_t fmt, int access, int w, int h);
    void transitionLoad(SDL_Surface* tr, uint8_t frame, SDL_Texture* dst);
private:
    std::multimap<int, GS_RenderInfo> renderinfo_;
    
public:
    void renderClear();
    void renderInsert(int z, GS_RenderInfo& info);
    // Replace screen to _renderinfo list and clear _renderinfo.
    void renderPresent();

    // GS_System::renderToBuffer()
    // Render the render info list into buffer(for transition, screenshot, etc.)
    void renderToBuffer();

    // GS_System::renderToScreen()
    // Render the render info list directly onto screen
    void renderToScreen();

    // GS_System::renderFromBufferToScreen()
    // Render the buffer into screen
    void renderBufferToScreen();
    void renderBufferToScreen(SDL_Texture* transition);

};
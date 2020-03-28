#pragma once
/* ----------------------------------------------------------/
 * scene.h
 * ----------------------------------------------------------/
 * Class declarations of scenes and scene objects.
 * ---------------------------------------------------------*/

#include <memory>

#include "basic_sdl.h"
#include "graphics.h"
#include "macros.h"
#include "singleton.h"

class GS_Scene
{
public:
    virtual ~GS_Scene() {}
    virtual void initLoad() = 0;
    virtual void refresh() = 0;
};

class GS_SceneManager
{
    DEFINE_SINGLETON(GS_SceneManager)
public:
    using UPScene = std::unique_ptr<GS_Scene>;
    void mainLoop();
    void setScene(UPScene newScene);
private:
    UPScene scene_;

};
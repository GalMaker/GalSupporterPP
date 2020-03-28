#pragma once
/* ----------------------------------------------------------/
 * uiscene.h
 * ----------------------------------------------------------/
 * Base classes for UI Scenes.
 * ---------------------------------------------------------*/
#include "scene.h"
#include <functional>

class GS_UIScene : public GS_Scene
{
public:
    struct GS_FuncOnMsg
    {
        enum MsgType
        {
            MsgOnClick,
            MsgOnRightClick,
            MsgOnMouseOver
        };
        using MsgFunc = std::function<void(void)>;
        MsgFunc onClick, onRightClick;
        MsgFunc onMouseOver;
        GS_FuncOnMsg(
            const MsgFunc& onClick_ = nullptr,
            const MsgFunc& onRightClick_ = nullptr,
            const MsgFunc& onMouseOver_ = nullptr)
            : onClick(onClick_), onRightClick(onRightClick_), onMouseOver(onMouseOver_)
        {}
    };
    using GS_MsgType = GS_FuncOnMsg::MsgType;
    class GS_UIObject
    {
    public:
        GS_FuncOnMsg funcOnMsg;
        GS_UIObject() : funcOnMsg() {}
        GS_UIObject(GS_FuncOnMsg msg) : funcOnMsg(msg) {}
        virtual void initLoad() = 0;
        virtual void refresh() = 0;


    };
    class GS_UIButton : public GS_UIObject
    {
    public:
        GS_UIButton(const std::string& filePath, )
    };
    
};
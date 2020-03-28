#pragma once
/* ----------------------------------------------------------/
 * setting.h
 * ----------------------------------------------------------/
 *
 * ---------------------------------------------------------*/

#include <string>
#include <SDL.h>
#include "singleton.h"
#include "picojson.h"

class GS_Settings
{
    DEFINE_SINGLETON(GS_Settings)
private:
    using pv_ = picojson::value;
    picojson::object _settings =
    {
        { "win_x", pv_(100.0) },
        { "win_y", pv_(100.0) },
        { "win_w", pv_(1280.0) },
        { "win_h", pv_(720.0) },
        { "baseline_w", pv_(1280.0) },
        { "baseline_h", pv_(720.0) },
        { "fullscreen", pv_(false) },
        { "borderless", pv_(true) },
        { "win_title", pv_("GS++ Pre-Alpha") }
    };
    inline int getJSONInt(picojson::object& obj, const std::string& id)
    {
        return static_cast<int>(obj[id].get<double>());
    }
public:
    int winX()        { return getJSONInt(_settings, "win_x"); }
    int winY()        { return getJSONInt(_settings, "win_y"); }
    int winW()        { return getJSONInt(_settings, "win_w"); }
    int winH()        { return getJSONInt(_settings, "win_h"); }
    int baselineW()   { return getJSONInt(_settings, "baseline_w"); }
    int baselineH()   { return getJSONInt(_settings, "baseline_h"); }
    int scaleW(int src)      { return src * winW() / baselineW(); }
    int scaleH(int src)      { return src * winH() / baselineH(); }
    int invScaleW(int src)   { return src * baselineW() / winW(); }
    int invScaleH(int src)   { return src * baselineH() / winH(); }
    const std::string& winTitle() { return _settings["win_title"].get<std::string>(); }
    bool fullscreen() { return _settings["fullscreen"].get<bool>(); }
    bool borderless() { return _settings["borderless"].get<bool>(); }
};
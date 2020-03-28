#pragma once
/* ----------------------------------------------------------/
 * audio.h
 * ----------------------------------------------------------/
 * Audio device handlers.
 * ---------------------------------------------------------*/

#include <string>
#include <map>
#include <SDL.h>
#include <SDL_mixer.h>
#include "singleton.h"
#include <plog/Log.h>

class GS_AudioClip
{
public:
    virtual ~GS_AudioClip() { }
    virtual void fadein(int channel, int loops, int time_ms) = 0;
    virtual void play(int channel, int loops) = 0;
    virtual void load() = 0;
    virtual void unload() = 0;
};
class GS_MusicClip : public GS_AudioClip
{
private:
    Mix_Music* mus_ = nullptr;
    std::string path_;
public:
    GS_MusicClip(const std::string& path);
    ~GS_MusicClip();
    void fadein(int channel, int loops, int time_ms);
    void play(int channel, int loops);
    void load() {}
    void unload() {}
};
class GS_BufferedClip : public GS_AudioClip
{
private:
    Mix_Chunk* clip_ = nullptr;
    std::string path_;
public:
    GS_BufferedClip(const std::string& path);
    ~GS_BufferedClip();
    void fadein(int channel, int loops, int time_ms);
    void play(int channel, int loops);
    void load();
    void unload();
};
enum GS_AudioChannel
{
    GS_CHNL_BGM,
    GS_CHNL_BGS,
    GS_CHNL_ME,
    GS_CHNL_SE
};
class GS_Audio
{
    DEFINE_SINGLETON(GS_Audio)

public:
    using ClipPtr = std::unique_ptr<GS_AudioClip>;
    
    void reg(const std::string& id, ClipPtr ptr);
    void fadein(const std::string& id, int channel, int loops, int time_ms);
    void play(const std::string& id, int channel, int loops);
    void load(const std::string& id);
    void unload(const std::string& id);
    void pause(int channel);

private:
    std::map<std::string, ClipPtr> clips_;
};
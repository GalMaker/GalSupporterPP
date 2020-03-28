#include "audio.h"

GS_MusicClip::GS_MusicClip(const std::string& path)
	: path_(path)
{
	mus_ = Mix_LoadMUS(path.c_str());
	if (!mus_)
	{
		LOG_DEBUG << "GS_MusicClip::initializer(): Failed to load \""
			<< path << "\". SDL Error: " << Mix_GetError();
		return;
	}
}

GS_MusicClip::~GS_MusicClip()
{
	if (mus_)
		Mix_FreeMusic(mus_);
}

void GS_MusicClip::fadein(int channel, int loops, int time_ms)
{
	if (!mus_)
	{
		LOG_DEBUG << "GS_MusicClip::initializer(): \""
			<< path_ << "\" is not loaded! SDL Error: "
			<< Mix_GetError();
		return;
	}
	Mix_FadeInMusic(mus_, loops, time_ms);
}

void GS_MusicClip::play(int channel, int loops)
{
	if (!mus_)
	{
		LOG_DEBUG << "GS_MusicClip::initializer(): \""
			<< path_ << "\" is not loaded! SDL Error: "
			<< Mix_GetError();
		return;
	}
	Mix_PlayMusic(mus_, loops);
}

GS_BufferedClip::GS_BufferedClip(const std::string& path)
	: path_(path)
{
	// Nothing!
}

GS_BufferedClip::~GS_BufferedClip()
{
	if (clip_) Mix_FreeChunk(clip_);
}

void GS_BufferedClip::fadein(int channel, int loops, int time_ms)
{
	if (!clip_) load();
	if (clip_) Mix_FadeInChannel(channel, clip_, loops, time_ms);
}

void GS_BufferedClip::play(int channel, int loops)
{
	if (!clip_) load();
	if (clip_) Mix_PlayChannel(channel, clip_, loops);
}

void GS_BufferedClip::load()
{
	if (!clip_)
	{
		clip_ = Mix_LoadWAV(path_.c_str());
		if (!clip_)
		{
			LOG_DEBUG << "GS_MusicClip::initializer(): \""
				<< path_ << "\" is not loaded! SDL Error: "
				<< Mix_GetError();
			return;
		}
	}
}

void GS_BufferedClip::unload()
{
	if (clip_) Mix_FreeChunk(clip_);
	clip_ = nullptr;
}

void GS_Audio::reg(const std::string& id, GS_Audio::ClipPtr ptr)
{
	clips_.emplace(id, std::move(ptr));
}

void GS_Audio::fadein(const std::string& id, int channel, int loops, int time_ms)
{
	if (!clips_.contains(id))
	{
		LOG_DEBUG << "GS_Audio::fadein(): Clip \""
			<< id << "\" do not exist.";
		return;
	}
	clips_[id]->fadein(channel, loops, time_ms);
}

void GS_Audio::play(const std::string& id, int channel, int loops)
{
	if (!clips_.contains(id))
	{
		LOG_DEBUG << "GS_Audio::play(): Clip \""
			<< id << "\" do not exist.";
		return;
	}
	clips_[id]->play(channel, loops);
}

void GS_Audio::load(const std::string& id)
{
	if (!clips_.contains(id))
	{
		LOG_DEBUG << "GS_Audio::load(): Clip \""
			<< id << "\" do not exist.";
		return;
	}
	clips_[id]->load();
}

void GS_Audio::unload(const std::string& id)
{
	if (!clips_.contains(id))
	{
		LOG_DEBUG << "GS_Audio::unload(): Clip \""
			<< id << "\" do not exist.";
		return;
	}
	clips_[id]->unload();
}

void GS_Audio::pause(int channel)
{
	if (channel == GS_CHNL_BGM) 
		Mix_PauseMusic();
	else 
		Mix_Pause(channel);
}

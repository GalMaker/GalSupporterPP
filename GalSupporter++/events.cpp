#include "events.h"

void GS_Events::refresh()
{
	events_.clear();
	SDL_Event curEvent;
	while (SDL_PollEvent(&curEvent))
	{
		events_.emplace_back(curEvent);
	}
}

bool GS_Events::keyDown(SDL_Scancode code, bool clear)
{
	for (auto it = events_.begin(); it != events_.end(); ++it)
	{
		if ((it->type == SDL_EventType::SDL_KEYDOWN) 
			&& (it->key.keysym.scancode == code))
		{
			if (clear) events_.erase(it);
			return true;
		}
	}
	return false;
}

bool GS_Events::keyUp(SDL_Scancode code, bool clear)
{
	for (auto it = events_.begin(); it != events_.end(); ++it)
	{
		if ((it->type == SDL_EventType::SDL_KEYUP)
			&& (it->key.keysym.scancode == code))
		{
			if (clear) events_.erase(it);
			return true;
		}
	}
	return false;
}

bool GS_Events::mouseLDown(GS_Events::RangeFunc range, bool clear)
{
	for (auto it = events_.begin(); it != events_.end(); ++it)
	{
		if ((it->type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
			&& (it->button.button == SDL_BUTTON_LEFT)
			&& (range(GS_Tools::rawToBaseline(SDL_Point({it->button.x, it->button.y})))))
		{
			if (clear) events_.erase(it);
			return true;
		}
	}
	return false;
}
bool GS_Events::mouseLUp(GS_Events::RangeFunc range, bool clear)
{
	for (auto it = events_.begin(); it != events_.end(); ++it)
	{
		if ((it->type == SDL_EventType::SDL_MOUSEBUTTONUP)
			&& (it->button.button == SDL_BUTTON_LEFT)
			&& (range(GS_Tools::rawToBaseline(SDL_Point({ it->button.x, it->button.y }))))
			)
		{
			if (clear) events_.erase(it);
			return true;
		}
	}
	return false;
}

bool GS_Events::windowClose(bool clear)
{
	for (auto it = events_.begin(); it != events_.end(); ++it)
	{
		if (it->type == SDL_EventType::SDL_WINDOWEVENT
			&& it->window.type == SDL_WINDOWEVENT_CLOSE)
		{
			if (clear) events_.erase(it);
			return true;
		}
	}
	return false;
}

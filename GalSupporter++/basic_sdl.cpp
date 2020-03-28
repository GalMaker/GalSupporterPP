#include "basic_sdl.h"
#include "plog/Log.h"
#include "macros.h"
#include <functional>
#include <utility>
using  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3;

GS_System::GS_System()
{
	// Initialize Plog Lib
	plog::init(plog::debug, "gspp.log");

	// Initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		LOG_FATAL << "SDL_Init() Error! SDL Error code:" << SDL_GetError();
		exit(EXIT_FAILURE);
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
	{
		LOG_FATAL << "IMG_Init() Error! SDL Error code:" << IMG_GetError();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	if (TTF_Init() < 0)
	{
		LOG_FATAL << "TTF_Init() Error! SDL Error code:" << TTF_GetError();
		IMG_Quit();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_FLAC) == 0)
	{
		LOG_FATAL << "Mix_Init() Error! SDL Error code:" << Mix_GetError();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 128) == -1)
	{
		LOG_FATAL << "Mix_OpenAudio() Error! SDL Error code:" << Mix_GetError();
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	// Initialize Window
	int windowflags =
		(GSSET.borderless() ? SDL_WINDOW_BORDERLESS : NULL) |
		(GSSET.fullscreen() ? SDL_WINDOW_FULLSCREEN : NULL);
	if ((window_ = SDL_CreateWindow(
						GSSET.winTitle().c_str(),
						GSSET.winX(),
						GSSET.winY(),
						GSSET.winW(),
						GSSET.winH(),
						windowflags)) == nullptr)
	{
		LOG_FATAL << "SDL_CreateWindow() Error! SDL Error code:" << SDL_GetError();
		Mix_CloseAudio();
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	// Initialize Renderer
	if ((renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE))
			== nullptr)
	{
		LOG_FATAL << "SDL_CreateRenderer() Error! SDL Error code:" << SDL_GetError();
		SDL_DestroyWindow(window_);
		Mix_CloseAudio();
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	// Test code
	texbuf_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GSSET.winW(), GSSET.winH());
}

GS_System::~GS_System()
{
	if (texbuf_) SDL_DestroyTexture(texbuf_);
	if (renderer_) SDL_DestroyRenderer(renderer_);
	if (window_) SDL_DestroyWindow(window_);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* GS_System::imgLoad(const std::string& path)
{
	return IMG_LoadTexture(renderer_, path.c_str());
}

SDL_Texture* GS_System::textLoad(const std::u8string& utf8string, TTF_Font* font, SDL_Color color, Uint32 width)
{
	SDL_Surface* sur_ = TTF_RenderUTF8_Blended_Wrapped(
		font,
		(const char*)(utf8string.c_str()), color, width);
	SDL_Texture* tex_ = SDL_CreateTextureFromSurface(renderer_, sur_);
	SDL_FreeSurface(sur_);
	return tex_;
}

SDL_Texture* GS_System::surfaceLoad(SDL_Surface* sur)
{
	if (!sur)
	{
		LOG_WARNING << "GS_System::surfaceLoad(): Unexpected surface. SDL Error: "
			<< SDL_GetError();
		return nullptr;
	}
	return SDL_CreateTextureFromSurface(renderer_, sur);
}

SDL_Texture* GS_System::transitionLoad(SDL_Surface* tr, uint8_t frame)
{
	SDL_Surface* tr_filtered =
		SDL_CreateRGBSurface(0, tr->w, tr->h, 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_memset(tr_filtered->pixels, 0, tr_filtered->pitch * tr->h);

	//SDL_LockSurface(tr);
	//SDL_LockSurface(tr_filtered);



	for (int i = 0; i < (tr->w * tr->h); ++i)
	{
		uint32_t curPixel = 0;
		int offset = i / (tr->w) * tr->pitch
			+ (i % (tr->w)) * (tr->format->BytesPerPixel);
		memcpy(&curPixel, (uint8_t*)(tr->pixels) + offset, tr->format->BytesPerPixel);
		uint8_t index = 0, dummy = 0;
		SDL_GetRGBA(curPixel, tr->format, &index, &dummy, &dummy, &dummy);
		if (index >= frame)
		{
			((uint32_t*)(tr_filtered->pixels))[i] = 0xffffffff;
		}
		else
		{
			((uint32_t*)(tr_filtered->pixels))[i] = 0xff000000;
		}
	}
	//SDL_UnlockSurface(tr_filtered);
	
	//SDL_UnlockSurface(tr);
	//SDL_SaveBMP(tr_filtered, "test.bmp");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, tr_filtered);
	SDL_FreeSurface(tr_filtered);
	return tex;
}

void GS_System::renderClear()
{
	renderinfo_.clear();
}

void GS_System::renderInsert(int z, GS_RenderInfo& info)
{
	renderinfo_.emplace(z, info);
}


void GS_System::renderPresent()
{
	SDL_SetRenderTarget(renderer_, nullptr);
	SDL_RenderCopy(renderer_, texbuf_, nullptr, nullptr);
	SDL_RenderPresent(renderer_);
}

void GS_System::renderToBuffer()
{
	SDL_SetRenderTarget(renderer_, texbuf_);
	SDL_RenderClear(renderer_);

	for (auto& it_ri : renderinfo_)
	{
		SDL_RenderCopyEx(
			renderer_,
			it_ri.second.tex,
			&it_ri.second.srcRect,
			&it_ri.second.dstRect,
			it_ri.second.angle,
			nullptr,
			SDL_FLIP_NONE);
	}

	SDL_RenderPresent(renderer_);
}

void GS_System::renderToScreen()
{
	SDL_SetRenderTarget(renderer_, nullptr);
	SDL_RenderClear(renderer_);

	for (auto& it_ri : renderinfo_)
	{
		SDL_RenderCopyEx(
			renderer_,
			it_ri.second.tex,
			&it_ri.second.srcRect,
			&it_ri.second.dstRect,
			it_ri.second.angle,
			nullptr,
			SDL_FLIP_NONE);
	}

	SDL_RenderPresent(renderer_);
}

void GS_System::renderBufferToScreen()
{
	SDL_SetRenderTarget(renderer_, nullptr);
	SDL_RenderCopy(renderer_, texbuf_, nullptr, nullptr);
	SDL_RenderPresent(renderer_);
}

void GS_System::renderBufferToScreen(SDL_Texture* transition)
{
	if (!transition)
	{
		LOG_WARNING << "GS_System::renderBufferToScreen_transition(): texture do not exist.";
		return;
	}

}

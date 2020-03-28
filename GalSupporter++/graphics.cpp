#include "graphics.h"
#include "macros.h"
#include <plog/Log.h>

void ImageTexture::render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1)
{
	if (!tex_) load();
	if (!tex_) return;
	if (srcRect.w < 0) srcRect.w = w_;
	if (srcRect.h < 0) srcRect.h = h_;
	if (dstRect.w < 0) dstRect.w = w_;
	if (dstRect.h < 0) dstRect.h = h_;
	
	GS_RenderInfo info = { tex_, srcRect, dstRect, angle };
	GSSYS.renderInsert(z, info);
}

void ImageTexture::reload()
{
	unload();
	load();
}

void ImageTexture::load()
{
	if (!tex_) tex_ = GSSYS.imgLoad(path_);
	SDL_QueryTexture(tex_, nullptr, nullptr, &w_, &h_);
	if (!tex_)
	{
		LOG_WARNING << "ImageTexture::load() Error: Unable to load " << path_ << ".";
	}
}

void ImageTexture::unload()
{
	if (tex_)	SDL_DestroyTexture(tex_);
	tex_ = nullptr;
}

void TiledTexture::render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1)
{
	int x_index = (param0 % frames_) % xdiv_;
	int y_index = (param0 % frames_) / xdiv_;
	SDL_Rect realSrcRect = {
		srcRect.x + w() * x_index,
		srcRect.y + h() * y_index,
		srcRect.w < 0 ? (w() - srcRect.x) : srcRect.w,
		srcRect.h < 0 ? (h() - srcRect.y) : srcRect.h
	};
	SDL_Rect realDstRect = {
		dstRect.x,
		dstRect.y,
		dstRect.w < 0 ? w() : dstRect.w,
		dstRect.h < 0 ? h() : dstRect.h
	};
	basicimg_->render(realSrcRect, realDstRect, z, angle, 0, 0);
}

void GS_Graphics::regTexture(const std::string& id, std::unique_ptr<TextureBasic> texture)
{
	textures_.try_emplace(id, std::move(texture));
}

void GS_Graphics::loadTexture(const std::string& id)
{
	if (!textures_.contains(id))
	{
		LOG_WARNING
			<< "GS_Graphics::loadTexture(): Failed to load \""
			<< id
			<< "\". Texture ID does not exist.";
		return;
	}
	textures_[id]->load();
}


void GS_Graphics::unloadTexture(const std::string& id)
{
	if (!textures_.contains(id))
	{
		LOG_WARNING
			<< "GS_Graphics::loadTexture(): Failed to unload \""
			<< id
			<< "\". Texture ID does not exist.";
		return;
	}
	textures_[id]->unload();
}

void GS_Graphics::reloadTexture(const std::string& id)
{
	if (!textures_.contains(id))
	{
		LOG_WARNING
			<< "GS_Graphics::loadTexture(): Failed to reload \""
			<< id
			<< "\". Texture ID does not exist.";
		return;
	}
	textures_[id]->reload();
}

void GS_Graphics::regFont(const std::string& id, const std::string& path, int ptSize)
{
	auto font = TTF_OpenFont(path.c_str(), ptSize);
	if (!font) 
	{
		LOG_WARNING << "GS_Graphics::regFont(): Failed to load font from "
			<< path << ".";
		return;
	}
	auto&& pair = fonts_.try_emplace(id, font);
}


void GS_Graphics::genTextTexture(const std::string& id, const std::string& fontid, std::u8string& str_ref, SDL_Color& color_ref, Uint32 width)
{
	textures_.try_emplace(id, new TextTexture(fonts_[fontid], str_ref, color_ref, width));
}

void GS_Graphics::renderTexture(const std::string& id, SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1)
{
	if (!textures_.contains(id))
	{
		LOG_WARNING
			<< "GS_Graphics::loadTexture(): Failed to render \""
			<< id
			<< "\". Texture ID does not exist.";
		return;
	}
	textures_[id]->render(srcRect, dstRect, z, angle, param0, param1);
}

void SpriteTexture::render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1)
{
	if (!rectmap_.contains(param0))
	{
		LOG_WARNING << "SpriteTexture::render(): key \"" << param0 << "\" not found.";
		return;
	}
	SDL_Rect& spriteRect = rectmap_[param0];
	SDL_Rect realSrcRect = {
		srcRect.x,
		srcRect.y,
		srcRect.w < 0 ? (spriteRect.w - srcRect.x) : srcRect.w,
		srcRect.h < 0 ? (spriteRect.h - srcRect.y) : srcRect.h
	};
	SDL_Rect realDstRect = {
		dstRect.x,
		dstRect.y,
		dstRect.w < 0 ? spriteRect.w : dstRect.w,
		dstRect.h < 0 ? spriteRect.h : dstRect.h
	};
}


void TextTexture::render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1)
{
	if (!tex_) load();
	if (!tex_) return;
	if (srcRect.w < 0) srcRect.w = w_;
	if (srcRect.h < 0) srcRect.h = h_;
	if (dstRect.w < 0) dstRect.w = w_;
	if (dstRect.h < 0) dstRect.h = h_;

	GS_RenderInfo info = { tex_, srcRect, dstRect, angle };
	GSSYS.renderInsert(z, info);
}

void TextTexture::load()
{
	tex_ = GSSYS.textLoad(str_, font_, color_, width_);
	SDL_QueryTexture(tex_, nullptr, nullptr, &w_, &h_);
	if (!tex_)
	{
		LOG_WARNING << "TextTexture::load(): Failed to render text into texture.";
	}
}

void TextTexture::unload()
{
	if (tex_)
	{
		SDL_DestroyTexture(tex_);
		tex_ = nullptr;
	}
}

void TransitionTexture::render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1)
{

	if (!sur_orig) load();
	if (tex_) SDL_DestroyTexture(tex_);
	tex_ = GSSYS.transitionLoad(sur_orig, param0 % 256);
	SDL_SetTextureBlendMode(tex_, SDL_BLENDMODE_BLEND);



#if 0
	uint8_t framei = param0 % 256;
	uint32_t framemask = framei * 0x01010101;

	if (!sur_orig) load();

	SDL_Surface* sur_now = 
		SDL_CreateRGBSurface(0, sur_orig->w, sur_orig->h, sur_orig->format->BitsPerPixel,
			sur_orig->format->Rmask, sur_orig->format->Gmask, sur_orig->format->Bmask, sur_orig->format->Amask);
	if (!sur_now)
	{
		LOG_WARNING << "TransitionTexture::render(): Failed to create surface! SDL Error:"
			<< SDL_GetError();
		return;
	}
	SDL_LockSurface(sur_orig);
	SDL_LockSurface(sur_now);
	uint32_t* src_pixels = reinterpret_cast<uint32_t*>(sur_orig->pixels);
	uint32_t* dst_pixels = reinterpret_cast<uint32_t*>(sur_now->pixels);

	for (int i = 0; i < sur_orig->w * sur_orig->h; ++i)
	{
		if (src_pixels[i] <= framemask) 
			dst_pixels[i] = 0xFFFFFFFF;
		else dst_pixels[i] = 0x00000000;
	}
	SDL_UnlockSurface(sur_now);
	SDL_UnlockSurface(sur_orig);

	if (tex_) SDL_DestroyTexture(tex_);
	tex_ = GSSYS.surfaceLoad(sur_now);
	SDL_FreeSurface(sur_now);
	SDL_SetTextureBlendMode(tex_, SDL_BLENDMODE_BLEND);
#endif

	if (srcRect.w < 0) srcRect.w = w_;
	if (srcRect.h < 0) srcRect.h = h_;
	if (dstRect.w < 0) dstRect.w = w_;
	if (dstRect.h < 0) dstRect.h = h_;

	GS_RenderInfo info = { tex_, srcRect, dstRect, angle };
	GSSYS.renderInsert(z, info);
}

void TransitionTexture::load()
{
	sur_orig = IMG_Load(path_.c_str());
	if (!sur_orig)
	{
		LOG_WARNING
			<< "TransitionTexture::load(): Failed to load \""
			<< path_
			<< "\". SDL Error: "
			<< IMG_GetError();
		return;
	}
}

void TransitionTexture::unload()
{
	if (sur_orig)
		SDL_FreeSurface(sur_orig);
}

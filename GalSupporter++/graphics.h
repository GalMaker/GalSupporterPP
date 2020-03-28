#pragma once
/* ----------------------------------------------------------/
 * graphics.h
 * ----------------------------------------------------------/
 * Texture Classes, Graphics handlers.
 * ---------------------------------------------------------*/
#include <string>
#include <memory>
#include "singleton.h"
#include "basic_sdl.h"
#include <plog/Log.h>

// TextureBasic class
// Unified Interface for all types of textures
class TextureBasic
{
public:
    virtual ~TextureBasic() {}

    // TextureBasic::render(srcRect, dstRect, z, angle, param0, param1)
    // insert this texture into render list
    // srcRect: rectangle of source texture to be rendered (w==-1=>full width, h==-1=>full height)
    // dstRect: rectangle of target in baseline (w==-1=>full width, h==-1=>full height)
    // z: z-axis info(simple layer system)
    // angle: for rotating(in RAD)
    // param0, param1: depend on specific type of texture
    virtual void render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1) = 0;

    // TextureBasic::w()
    // get-er of width
    virtual int w() = 0;
    // TextureBasic::h()
    // get-er of height
    virtual int h() = 0;

    // TextureBasic::reload()
    // syntax sugar of unload() and load()
    virtual void reload() = 0;
    virtual void load() = 0;
    virtual void unload() = 0;
};
class ImageTexture : public TextureBasic
{
private:
    SDL_Texture* tex_ = nullptr;
    int w_ = -1, h_ = -1;
    std::string path_;
public:
    ImageTexture(const std::string& path) : path_(path) {}
    ~ImageTexture() { if (tex_) unload(); }

    // ImageTexture::render(srcRect, dstRect, z, angle, param0, param1)
    // srcRect: rectangle of source texture to be rendered
    // dstRect: rectangle of target in baseline
    // z: z-axis info(simple layer system)
    // angle: for rotating(in RAD)
    // insert this texture into render list
    // param0 : unused
    // param1 : unused
    void render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1);

    // w(): get-er of width
    int w() { if (!tex_) load(); return w_; }
    // h(): get-er of height
    int h() { if (!tex_) load(); return h_; }

    // reload(): syntax sugar of unload() and load()
    void reload();
    // load(): Actually load texture into VRAM. For manual VRAM usage opt.
    void load();
    // unload(): unload texture from VRAM. For manual VRAM usage opt.
    void unload();
};
class TransitionTexture : public TextureBasic
{
private:
    SDL_Texture* tex_ = nullptr;
    SDL_Surface* sur_orig = nullptr;
    int w_ = -1, h_ = -1;
    std::string path_;
public:
    TransitionTexture(const std::string& path) : path_(path) {}
    ~TransitionTexture() { if (tex_) unload(); }
    void render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1);
    int w() { if (!tex_) load(); return w_; }
    int h() { if (!tex_) load(); return h_; }
    void reload() { unload(); load(); }
    void load();
    void unload();
};

// SpriteTexture class
// Description:
// A large piece of texture for ``packed" texture load.
// SpriteTexture == ImageTexture && (int -> rect) map
class SpriteTexture : public TextureBasic
{
private:
    std::unique_ptr<ImageTexture> basicimg_;
    std::map<int, SDL_Rect> rectmap_;
public:
    // SpriteTexture::SpriteTexture(path, rectmap)
    // path: image path(real relative or full path or packed virtual path<- TODO)
    // rectmap: mapped info of (index -> rect) relations
    SpriteTexture(const std::string& path, std::map<int, SDL_Rect> rectmap)
        : basicimg_(new ImageTexture(path)), rectmap_(rectmap) {}

    // SpriteTexture::render(srcRect, dstRect, z, angle, param0, param1)
    // srcRect: rectangle of source texture to be rendered
    // dstRect: rectangle of target in baseline
    // z: z-axis info(simple layer system)
    // angle: for rotating(in RAD)
    // param0: index
    // param1: unused
    void render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1);
    int w()         { return basicimg_->w(); }
    int h()         { return basicimg_->h(); }
    void reload()   { basicimg_->reload(); }
    void load()     { basicimg_->load(); }
    void unload()   { basicimg_->unload(); }
};
class TiledTexture : public TextureBasic
{
private:
    std::unique_ptr<ImageTexture> basicimg_;
    int xdiv_ = -1, ydiv_ = -1, frames_ = -1;
    // std::string path_;
public:
    TiledTexture(const std::string& path, int xdiv, int ydiv, int frames)
        : basicimg_(new ImageTexture(path)), xdiv_(xdiv), ydiv_(ydiv), frames_(frames) {}
    void render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1);
    int w() { return basicimg_->w() / xdiv_; }
    int h() { return basicimg_->h() / ydiv_; }
    void reload()   { basicimg_->reload(); }
    void load()     { basicimg_->load(); }
    void unload()   { basicimg_->unload(); }
};
class TextTexture : public TextureBasic
{
private:
    SDL_Texture* tex_ = nullptr;
    int w_ = -1, h_ = -1;
    std::u8string& str_;
    SDL_Color& color_;
    TTF_Font* font_ = nullptr;
    Uint32 width_;

    // TextTexture is designed not directly creatable by users.
    // Use member functions in GS_Graphics instead.
    friend class GS_Graphics;
    TextTexture(TextTexture&) = delete;
    TextTexture(const TextTexture&) = delete;
    TextTexture() = delete;
    // TTF_Font* type may change in future implementations
    TextTexture(TTF_Font* font, std::u8string& str_ref, SDL_Color& color_ref, Uint32 width)
        :font_(font), color_(color_ref), width_(width), str_(str_ref) {}
public:

    void render(SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1);
    int w() { if (!tex_) load(); return w_; }
    int h() { if (!tex_) load(); return h_; }
    void reload() { unload(); load(); }
    void load();
    void unload();

};
class GS_Graphics
{
    DEFINE_SINGLETON(GS_Graphics)
private:
    using TexturePtr = std::unique_ptr<TextureBasic>;
    using FontPtr = TTF_Font*;
    std::map<std::string, TexturePtr> textures_;
    std::map<std::string, FontPtr> fonts_;
public:

    // GS_Graphics::regFont(id, path, ptSize)
    // Register a TTF/OTF font file with specified size(not changable later).
    // id: font id(not in the same scope with texture id)
    // path: file path(relative/full/virtual)
    // ptSize: size in pt(s)
    void regFont(const std::string& id, const std::string& path, int ptSize);

    // GS_Graphics::genTextTexture(id, fontid, str_ref, color_ref, width)
    // generate text texture(UTF-8) to id with text[str_ref] in font[#fontid], color[color_ref]
    // id: target texture id
    // fontid: id of loaded font face(= TTF + size)
    // str_ref: ref of UTF-8 text string(can be changed later)
    // color_ref: ref of text color(can be changed later)
    // width: textbox width(auto-feed)
    void genTextTexture(const std::string& id, const std::string& fontid, std::u8string& str_ref, SDL_Color& color_ref, Uint32 width);

    // GS_Graphics::regTexture(id, texture)
    // Load a texture to id.
    // Usage: GSGRA.regTexture("the_id", TEX(ImageTexture,"picture.png"));
    // Syntax sugar of
    // GS_Graphics::instance()->regTexture("the_id", std::make_unique<ImageTexture>("picture.png"));
    void regTexture(const std::string& id, std::unique_ptr<TextureBasic> texture);

    // GS_Graphics::loadTexture(id)
    // Actually load texture into VRAM(if not loaded).
    // Notice: even if a texture is not loaded, it will load itself when render
    void loadTexture(const std::string& id);

    // GS_Graphics::unloadTexture(id)
    // Release a texture from VRAM(if loaded).
    // For manual VRAM optimization(maybe)
    void unloadTexture(const std::string& id);

    // GS_Graphics::reloadTexture(id)
    // Syntax sugar of unload->load
    // Designed to be mostly used for after changing text or color of a text texture.
    void reloadTexture(const std::string& id);

    // GS_Graphics::renderTexture(id, srcRect, dstRect, z, angle, param0, param1)
    // Insert a texture into render list(indexed by z)
    // id: texture id
    // srcRect: rect in texture to be rendered (w==-1=>full width, h==-1=>full height)
    // dstRect: rect in window baseline as render destination (w==-1=>full width, h==-1=>full height)
    // z:       render index(layer, higher=>closer to user)
    // angle:   clockwize rotation angle in RAD
    // param0, param1: external parameters depend on texture type
    void renderTexture(const std::string& id, SDL_Rect srcRect, SDL_Rect dstRect, int z, double angle, int param0, int param1);
};



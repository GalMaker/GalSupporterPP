#include "basic_sdl.h"
#include "graphics.h"
#include "events.h"
#include "audio.h"
#include "macros.h"

int main(int argc, char* argv[])
{
	GSSYS.create();
	GSEVT.refresh();
	std::u8string str = u8"あいうえおアイウエオ漢字0123";
	SDL_Color text_color = { 255,255,255,255 };
	GSGRA.regFont("default", "font01.otf", 24);
	GSGRA.genTextTexture("text1", "default", str, text_color, 1000u);
	SDL_Rect rectSrc = { 0, 0, -1, -1 };
	SDL_Rect rectDst = { 10, 10, -1, -1 };
	SDL_Rect rectDstS = { 5,5,300,300 };
	SDL_Rect rectDstBack = { 0,0,1280,720 };
	GSGRA.regTexture("tr01", TEX(TransitionTexture, "t02.png"));
	GSGRA.regTexture("shama", TEX(ImageTexture, "shama-lambda-icon.png"));
	GSGRA.regTexture("sora", TEX(ImageTexture, "falcom_sora.jpg"));

	GSAUD.reg("se_confirm", BUFCLIP("se_confirm.ogg"));
	int frames = 0;
	while (true)
	{
		GSSYS.renderClear();
		GSGRA.renderTexture("text1", rectSrc, rectDst, 100, 0.0, 0, 0);
		GSGRA.renderTexture("tr01", rectSrc, rectDst, 999999, 0.0, frames, 0);
		GSGRA.renderTexture("sora", rectSrc, rectDstBack, 1, 0.0, 0, 0);
		GSGRA.renderTexture("shama", rectSrc, rectDstS, 20, 5.0, 0, 0);
		//GSSYS.renderToBuffer();
		//GSSYS.renderPresent();
		GSSYS.renderToScreen();
		GSEVT.refresh();
		if (GSEVT.windowClose()) break;
		if (GSEVT.keyDown(SDL_SCANCODE_ESCAPE)) break;
		if (GSEVT.keyDown(SDL_SCANCODE_RETURN)) GSAUD.play("se_confirm", GS_CHNL_SE, 0);
		frames++;
	}

	return 0;
}
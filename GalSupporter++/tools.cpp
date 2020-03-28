#include "tools.h"
#include "macros.h"

SDL_Rect GS_Tools::baselineToRaw(SDL_Rect src)
{
	return {	
		GSSET.scaleW(src.x),
		GSSET.scaleH(src.y),
		GSSET.scaleW(src.w),
		GSSET.scaleH(src.h)
	};
}

SDL_Point GS_Tools::baselineToRaw(SDL_Point src)
{
	return {
		GSSET.scaleW(src.x),
		GSSET.scaleH(src.y)
	};
}
SDL_Rect GS_Tools::rawToBaseline(SDL_Rect src)
{
	return {
		GSSET.invScaleW(src.x),
		GSSET.invScaleH(src.y),
		GSSET.invScaleW(src.w),
		GSSET.invScaleH(src.h)
	};
}

SDL_Point GS_Tools::rawToBaseline(SDL_Point src)
{
	return {
		GSSET.invScaleW(src.x),
		GSSET.invScaleH(src.y)
	};
}

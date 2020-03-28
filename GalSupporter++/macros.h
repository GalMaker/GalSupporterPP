#pragma once

#define GSSET				(*GS_Settings::instance())
#define GSTLS				(*GS_Tools::instance())
#define GSSYS				(*GS_System::instance())
#define GSGRA				(*GS_Graphics::instance())
#define GSEVT				(*GS_Events::instance())
#define GSAUD				(*GS_Audio::instance())

#define TEX(typ_, tex_)		(std::make_unique<typ_>(tex_))
#define BUFCLIP(clip_)		(std::make_unique<GS_BufferedClip>(clip_))
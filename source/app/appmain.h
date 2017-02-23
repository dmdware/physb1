


#ifndef appmain_H
#define appmain_H

#include "../window.h"
#include "../texture.h"
#include "../math/fixfrac.h"

#define VERSION				1
#define TITLE				"Pencil"

int32_t HandleEvent(void *userdata, SDL_Event *e);
extern FixFrac* screendepths;
extern LoadedTex screencolors;

#endif

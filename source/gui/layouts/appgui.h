


#ifndef GGUI_H
#define GGUI_H

#include "../../math/vec3f.h"

class ViewLayer;
class Widget;

#if 0
extern bool g_canselect;
#endif

#define LEFT_PANEL_WIDTH	200

extern char g_lastsave[PENCIL_MAX_PATH+1];

void Resize_Fullscreen(Widget* w);
void Resize_FullscreenSq(Widget* w);
void Resize_AppLogo(Widget* w);
void Resize_AppTitle(Widget* w);
void Click_NewGame();
void Click_OpenEd();
void FillGUI();
void Click_LoadMapButton();
void Click_SaveMapButton();
void Click_QSaveMapButton();
void Resize_MenuItem(Widget* w);
void Resize_CenterWin(Widget* w);
void EnumDisplay();

#endif	//GGUI_H

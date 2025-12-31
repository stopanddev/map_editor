#ifndef left_menu_h
#define left_menu_h
#include "../configs/init.h"
#include "../system_utils/system_utils.h"
#include <stdio.h>

void Draw_back_panel(SDL_Renderer *renderer, AppState *appstate);
void Handle_left_panel_camera_input(AppState *appstate, SDL_Keycode key);
#endif

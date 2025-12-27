#ifndef left_menu_h
#define left_menu_h
#include "../configs/init.h"
#include "../system_utils/system_utils.h"
#include <stdio.h>

void draw_back_panel(SDL_Renderer *renderer, AppState *appstate);
void handle_left_panel_camera_input(AppState *appstate, SDL_Keycode key);
#endif

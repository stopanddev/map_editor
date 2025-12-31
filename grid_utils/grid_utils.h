#ifndef grid_utils_h
#define grid_utils_h
#include "../configs/init.h"
#include "../system_utils/system_utils.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <stdio.h>

#define XGRID_OFFSET 0
#define YGRID_OFFSET 0

void Init_camera(Camera *camera, int width, int height);
void Draw_grid(SDL_Renderer *renderer, AppState *appstate);
void Handle_camera_input(AppState *appstate, SDL_Keycode key);

#endif

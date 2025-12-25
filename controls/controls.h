#ifndef controls_h
#define controls_h
#include "../configs/init.h"
#include "../grid_utils/grid_utils.h"
#include "../motion_utils/motion_utils.h"
#include "../system_utils/system_utils.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <stdio.h>
SDL_AppResult handle_input(SDL_Event *event, AppState *appstate);
#endif

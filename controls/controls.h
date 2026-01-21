#ifndef controls_h
#define controls_h
#include "../configs/init.h"
#include "../grid_utils/grid_utils.h"
#include "../left_menu/left_menu.h"
#include "../motion_utils/motion_utils.h"
#include "../system_utils/system_utils.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <stdio.h>
SDL_AppResult Handle_input(SDL_Event *event, AppState *appstate);
void Motion_set_state(SDL_Keycode key, SDL_Keymod modState, AppState *appstate);
void Motion_event_handler(SDL_Keycode key, AppState *appstate);
void Reset_motion_state(AppState *appsate);
void Motion_state_event_handler(SDL_Keycode key, AppState *appstate);
void Motion_non_state_event_handler(SDL_Keycode key, AppState *appstate);
void Paste_tile_contents(AppState *appstate, bool x_row);
void Handle_grid_input(SDL_Keycode key, AppState *appstate);
void Handle_panel_input(SDL_Keycode key, AppState *appstate);
bool Check_direction(SDL_Keycode key);
void Check_numeric(SDL_Keycode key, AppState *appstate);
bool Is_numeric(SDL_Keycode key);
void Copy_direction(SDL_Keycode key, AppState *appstate);
void Copy_tile_to_cursor(int x, int y, int i, AppState *appstate);
#endif

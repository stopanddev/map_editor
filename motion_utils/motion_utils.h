#ifndef motion_h
#define motion_h
#include "../grid_utils/grid_utils.h"
#include <SDL3/SDL_init.h>
#include <math.h>
void draw_motion_cursor_grid(SDL_Renderer *renderer, AppState *appstate);
void draw_motion_cursor_panel(SDL_Renderer *renderer, AppState *appstate);
void draw_motion_mode_handler(SDL_Renderer *renderer, AppState *appstate);
void init_motion_cursor(MotionCursor *motionCursor);
void handle_motion_input(AppState *appstate, SDL_Keycode key);
#endif

#include "motion_utils.h"
#include <SDL3/SDL_init.h>

void init_motion_cursor(MotionCursor *motionCursor) {
  motionCursor->x = 0;
  motionCursor->y = 0;
}

void draw_motion_cursor(SDL_Renderer *renderer, AppState *appstate) {
  SDL_SetRenderDrawColor(renderer, 0, 50, 255, 155);
  int cursor_x = appstate->motionCursor.x;
  int cursor_y = appstate->motionCursor.y;

  float y = SDL_floor((cursor_y * TILE_PIXEL) + (cursor_y * GAP));
  float x = SDL_floor(MENU_WIDTH + (cursor_x * TILE_PIXEL) + (cursor_x * GAP));
  SDL_FRect square = {x, y, TILE_PIXEL, TILE_PIXEL};
  SDL_RenderRect(renderer, &square);
}

void handle_motion_input(AppState *appstate, SDL_Keycode key) {
  SDL_Keymod modState = SDL_GetModState();
  if (key == SDLK_LEFT || key == SDLK_J) {
    appstate->motionCursor.x -= 1;
  }
  if (key == SDLK_RIGHT || key == SDLK_L) {
    appstate->motionCursor.x += 1;
  }
  if (key == SDLK_UP || key == SDLK_I) {
    appstate->motionCursor.y -= 1;
  }
  if (key == SDLK_DOWN || key == SDLK_K) {
    appstate->motionCursor.y += 1;
  }
}

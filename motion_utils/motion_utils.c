#include "motion_utils.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <math.h>

void init_motion_cursor(MotionCursor *motionCursor) {
  motionCursor->x = 0;
  motionCursor->y = 0;
  motionCursor->gridx = 0;
  motionCursor->gridy = 0;
  motionCursor->panelx = 0;
  motionCursor->panely = 0;
}

void draw_motion_mode_handler(SDL_Renderer *renderer, AppState *appstate) {
  if (!appstate->motionCursor.mode) {
    draw_motion_cursor_grid(renderer, appstate);
  } else {
    draw_motion_cursor_panel(renderer, appstate);
  }
}
void draw_motion_cursor_grid(SDL_Renderer *renderer, AppState *appstate) {
  SDL_SetRenderDrawColor(renderer, 0, 50, 255, 155);
  int cursor_x = appstate->motionCursor.x;
  int cursor_y = appstate->motionCursor.y;

  float y = SDL_floor((cursor_y * TILE_PIXEL) + (cursor_y * GAP));
  float x = SDL_floor(MENU_WIDTH + (cursor_x * TILE_PIXEL) + (cursor_x * GAP));
  SDL_FRect square = {x, y, TILE_PIXEL, TILE_PIXEL};
  SDL_RenderRect(renderer, &square);
}

void draw_motion_cursor_panel(SDL_Renderer *renderer, AppState *appstate) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 135, 206, 255, 70);
  int cursor_x = appstate->motionCursor.x;
  int cursor_y = appstate->motionCursor.y;

  float y = SDL_floor((cursor_y * TILE_PIXEL));
  float x = SDL_floor((cursor_x * TILE_PIXEL));
  SDL_FRect square = {x, y, TILE_PIXEL, TILE_PIXEL};
  SDL_RenderFillRect(renderer, &square);
  // SDL_RenderRect (renderer, &square);
}

void handle_motion_input(AppState *appstate, SDL_Keycode key) {
  SDL_Keymod modState = SDL_GetModState();
  int maxX = 25;
  if (appstate->motionCursor.mode) {
    maxX = 6;
  }
  if (key == SDLK_LEFT || key == SDLK_H) {
    appstate->motionCursor.x =
        fmax(0, fmin(appstate->motionCursor.x -= 1, maxX));
  }
  if (key == SDLK_RIGHT || key == SDLK_L) {
    appstate->motionCursor.x =
        fmax(0, fmin(appstate->motionCursor.x += 1, maxX));
  }
  if (key == SDLK_UP || key == SDLK_K) {
    appstate->motionCursor.y = fmax(0, fmin(appstate->motionCursor.y -= 1, 22));
  }
  if (key == SDLK_DOWN || key == SDLK_J) {
    appstate->motionCursor.y = fmax(0, fmin(appstate->motionCursor.y += 1, 22));
  }
}

#include "grid_utils.h"
#include <SDL3/SDL_stdinc.h>

void DrawGrid(SDL_Renderer *renderer, AppState *appstate) {
  for (int r = 0; r <= MAP_HEIGHT; r++) {
    float y = SDL_floor((r * 32) + (r * GAP));
    for (int c = 0; c <= MAP_WIDTH; c++) {
      float x = SDL_floor(MENU_WIDTH + (c * 32) + (c * GAP));
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_FRect square = {x, y, 32, 32};

      SDL_RenderFillRect(renderer, &square);
    }
  }
}

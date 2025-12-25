#include "left_menu.h"
#include <SDL3/SDL_stdinc.h>

void draw_back_panel(SDL_Renderer *renderer, AppState *appstate) {
  SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
  SDL_FRect square = {0, 0, 160, 768};

  SDL_RenderFillRect(renderer, &square);
}

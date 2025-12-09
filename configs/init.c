
#include "init.h"

SDL_AppResult Init(AppState *state, int argc, char *argv[]) {

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
    SDL_Log("Couldn't initalize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Testing", SDL_WINDOW_WIDTH,
                                   SDL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                   &state->window, &state->renderer)) {
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(state->renderer, SDL_WINDOW_WIDTH,
                                   SDL_WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  state->last_step = SDL_GetTicks();
  state->cursor = SDL_GetDefaultCursor();

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      state->grid[r][c].textureId = 0;
      state->grid[r][c].collidable = 0;
    }
  }

  return SDL_APP_CONTINUE;
}

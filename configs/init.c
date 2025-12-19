
#include "init.h"

SDL_AppResult Init(AppState *state, int argc, char *argv[]) {

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Tile Map Editor", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                   &state->window, &state->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // --- Logical resolution setup (the important part) ---
  SDL_SetRenderLogicalPresentation(state->renderer, GAME_WIDTH, GAME_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  state->last_step = SDL_GetTicks();
  state->cursor = SDL_GetDefaultCursor();
  state->prevMouseButton = 0;

  // Initialize the logical grid
  for (int r = 0; r < MAP_HEIGHT; r++) {
    for (int c = 0; c < MAP_WIDTH; c++) {
      state->grid[r][c].textureId = 0;
      state->grid[r][c].collidable = 0;
    }
  }

  return SDL_APP_CONTINUE;
}

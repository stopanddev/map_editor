
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
  for (int c = 0; c < MAP_WIDTH; c++) {
    for (int r = 0; r < MAP_HEIGHT; r++) {
      state->grid[c][r].textureLoc.buffX = 0;
      state->grid[c][r].textureLoc.buffY = 0;
      state->grid[c][r].textureLoc.validTexture = false;
      state->grid[c][r].collidable = 0;
    }
  }
  Init_motion_state(state);
  return SDL_APP_CONTINUE;
}

void Init_motion_state(AppState *appstate) {
  appstate->motionState.ctrl_w = false;
  appstate->motionState.yank = false;
  appstate->motionState.modified = false;
}

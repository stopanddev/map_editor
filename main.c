#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include "configs/init.h"
#include "grid_utils/grid_utils.h"
#include "system_utils/system_utils.h"
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  AppState *state = SDL_calloc(1, sizeof(AppState));
  if (!state) {
    return SDL_APP_FAILURE;
  }
  *appstate = state;

  if (Init(state, argc, argv) != SDL_APP_CONTINUE) {
    return SDL_APP_FAILURE;
  } else {
    return SDL_APP_CONTINUE;
  }
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *as = (AppState *)appstate;
  const Uint64 now = SDL_GetTicks();
  float mouseX, mouseY;

  Uint32 mbutton = SDL_GetMouseState(&mouseX, &mouseY);
  SDL_FPoint mousePoint = {mouseX, mouseY};
  HandleMouseClick(as->renderer, as, mousePoint, mbutton);
  as->prevMouseButton = mbutton;
  // run game logic if we're at or past the time to run it.
  // if we're _really_ behind the time to run it, run it
  // several times.
  while ((now - as->last_step) >= STEP_RATE_MILLISECONDS) {
    as->last_step += STEP_RATE_MILLISECONDS;
  }
  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderClear(as->renderer);

  DrawGrid(as->renderer, as);
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_UP: {
    if (event->key.key == SDLK_S) {
      SaveMap(appstate);
    }
    if (event->key.key == SDLK_L) {
      LoadMap(appstate);
    }
    break;
  }
  defualt:
    break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (joystick) {
    SDL_CloseJoystick(joystick);
  }
  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
};

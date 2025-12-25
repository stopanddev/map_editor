#include "controls.h"

SDL_AppResult handle_input(SDL_Event *event, AppState *appstate) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN: {
    SDL_Keymod modState = SDL_GetModState();
    if (event->key.key == SDLK_S) {
      SaveMap(appstate);
    }
    if (modState == SDL_KMOD_CTRL && event->key.key == SDLK_L) {
      LoadMap(appstate);
    }
    if (event->key.key == SDLK_RIGHT || event->key.key == SDLK_LEFT ||
        event->key.key == SDLK_UP || event->key.key == SDLK_DOWN) {
      handle_camera_input(appstate, event->key.key);
      handle_motion_input(appstate, event->key.key);
    }
    if (event->key.key == SDLK_L || event->key.key == SDLK_J ||
        event->key.key == SDLK_I || event->key.key == SDLK_K) {
      handle_motion_input(appstate, event->key.key);
    }

    break;
  }
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

#include "controls.h"
#include <SDL3/SDL_keycode.h>
#include <linux/limits.h>

SDL_AppResult handle_input(SDL_Event *event, AppState *appstate) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN: {
    SDL_Keymod modState = SDL_GetModState();
    // Don't move the grid if not 'focused' on grid
    if ((!appstate->motionCursor.mode) &&
        (event->key.key == SDLK_RIGHT || event->key.key == SDLK_LEFT ||
         event->key.key == SDLK_UP || event->key.key == SDLK_DOWN)) {
      handle_camera_input(appstate, event->key.key);
      handle_motion_input(appstate, event->key.key);
    }
    if (event->key.key == SDLK_L || event->key.key == SDLK_J ||
        event->key.key == SDLK_H || event->key.key == SDLK_K) {
      handle_motion_input(appstate, event->key.key);
      if (appstate->motionCursor.mode) {
        handle_left_panel_camera_input(appstate, event->key.key);
      }
    }
    motion_event_handler(event->key.key, appstate);
    motion_set_state(event->key.key, modState, appstate);
    break;
  }
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

void motion_set_state(SDL_Keycode key, SDL_Keymod modState,
                      AppState *appstate) {
  if (modState == SDL_KMOD_LCTRL) {
    switch (key) {
      // Toggle between panel and grid
    case SDLK_W: {
      appstate->motionState.ctrl_w = true;
      break;
    }
    default:
      break;
    }
  }
}

void motion_event_handler(SDL_Keycode key, AppState *appstate) {
  MotionState temp_state = appstate->motionState;
  if (temp_state.ctrl_w) {
    switch (key) {
    case SDLK_W: {
      if (!appstate->motionCursor.mode) {
        appstate->motionCursor.gridx = appstate->motionCursor.x;
        appstate->motionCursor.gridy = appstate->motionCursor.y;
        appstate->motionCursor.x = appstate->motionCursor.panelx;
        appstate->motionCursor.y = appstate->motionCursor.panely;
      } else {
        appstate->motionCursor.panelx = appstate->motionCursor.x;
        appstate->motionCursor.panely = appstate->motionCursor.y;
        appstate->motionCursor.x = appstate->motionCursor.gridx;
        appstate->motionCursor.y = appstate->motionCursor.gridy;
      }
      appstate->motionCursor.mode = !appstate->motionCursor.mode;
      break;
    }
    default: {
      reset_motion_state(appstate);
      break;
    }
    }
  }
}

void reset_motion_state(AppState *appsate) {
  appsate->motionState.ctrl_w = false;
}

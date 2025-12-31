#include "controls.h"
#include <SDL3/SDL_keycode.h>
#include <linux/limits.h>

SDL_AppResult Handle_input(SDL_Event *event, AppState *appstate) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN: {
    SDL_Keymod modState = SDL_GetModState();
    // Don't move the grid if not 'focused' on grid
    if ((!appstate->motionCursor.mode) &&
        (event->key.key == SDLK_RIGHT || event->key.key == SDLK_LEFT ||
         event->key.key == SDLK_UP || event->key.key == SDLK_DOWN)) {
      Handle_camera_input(appstate, event->key.key);
      Handle_motion_input(appstate, event->key.key);
    }
    if (event->key.key == SDLK_L || event->key.key == SDLK_J ||
        event->key.key == SDLK_H || event->key.key == SDLK_K) {
      Handle_motion_input(appstate, event->key.key);
      if (appstate->motionCursor.mode) {
        Handle_left_panel_camera_input(appstate, event->key.key);
      }
    }
    Motion_event_handler(event->key.key, appstate);
    Motion_set_state(event->key.key, modState, appstate);
    break;
  }
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

void Motion_set_state(SDL_Keycode key, SDL_Keymod modState,
                      AppState *appstate) {
  if (modState == SDL_KMOD_LCTRL) {
    switch (key) {
      // Toggle between panel and grid
    case SDLK_W: {
      appstate->motionState.ctrl_w = true;
      appstate->motionState.modified = true;
      break;
    }
    default:
      break;
    }
  }
  if (key == SDLK_Y) {
    appstate->motionState.yank = true;
    appstate->motionState.modified = true;
  }
}

void Motion_event_handler(SDL_Keycode key, AppState *appstate) {
  Motion_state_event_handler(key, appstate);
  Motion_non_state_event_handler(key, appstate);
}

void Motion_state_event_handler(SDL_Keycode key, AppState *appstate) {
  MotionState temp_state = appstate->motionState;
  if (temp_state.ctrl_w) {
    switch (key) {
      // Switch between panel and grid
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
      Reset_motion_state(appstate);
      break;
    }
    default: {
      Reset_motion_state(appstate);
      break;
    }
    }
  } else if (temp_state.yank) {
    switch (key) {
    case SDLK_Y: {
      printf("THIS IS WHERE YANKING A LINE LOGIC GOES\n");
      break;
    }
    default: {
      int camxmod = appstate->cameraPanel.x;
      int camymod = appstate->cameraPanel.y;
      appstate->motionCursor.tileBuffer.buffX =
          appstate->motionCursor.x + camxmod;
      appstate->motionCursor.tileBuffer.buffY =
          appstate->motionCursor.y + camymod;
      Reset_motion_state(appstate);
      break;
    }
    }
  } else {
    Reset_motion_state(appstate);
  }
}

void Motion_non_state_event_handler(SDL_Keycode key, AppState *appstate) {
  switch (key) {
  case SDLK_P: {
    if (!appstate->motionCursor.mode) {
      Paste_tile_contents(appstate);
    }
  }
  }
}

void Paste_tile_contents(AppState *appstate) {
  AppState *as = (AppState *)appstate;
  as->grid[as->motionCursor.x][as->motionCursor.y].textureLoc.buffX =
      as->motionCursor.tileBuffer.buffX;
  as->grid[as->motionCursor.x][as->motionCursor.y].textureLoc.buffY =
      as->motionCursor.tileBuffer.buffY;
  as->grid[as->motionCursor.x][as->motionCursor.y].textureLoc.validTexture =
      true;
}

void Reset_motion_state(AppState *appstate) {
  if (appstate->motionState.modified) {
    appstate->motionState.ctrl_w = false;
    appstate->motionState.yank = false;
    appstate->motionState.modified = false;
  }
}

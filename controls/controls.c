#include "controls.h"
#include <SDL3/SDL_keycode.h>
#include <linux/limits.h>

SDL_AppResult Handle_input(SDL_Event *event, AppState *appstate) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN: {
    SDL_Keymod modState = SDL_GetModState();
    Check_numeric(event->key.key, appstate);
    // Don't move the grid if not 'focused' on grid
    if (!appstate->motionCursor.mode && !appstate->motionState.yank &&
        !appstate->motionState.pasting) {
      Handle_grid_input(event->key.key, appstate);
    } else if (!appstate->motionState.yank) {
      Handle_panel_input(event->key.key, appstate);
    }
    Motion_event_handler(event->key.key, appstate);
    // This is to prevent multiple states aside from count
    if (!appstate->motionState.modified) {
      Motion_set_state(event->key.key, modState, appstate);
    }
    break;
  }
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

void Handle_grid_input(SDL_Keycode key, AppState *appstate) {
  if (Check_direction(key) && !appstate->motionState.pasting) {
    printf("Is the check passing\n");
    Handle_camera_input(appstate, key);
    Handle_motion_input(appstate, key);
  }
}

void Handle_panel_input(SDL_Keycode key, AppState *appstate) {
  if (Check_direction(key)) {
    Handle_motion_input(appstate, key);
    Handle_left_panel_camera_input(appstate, key);
  }
}

bool Check_direction(SDL_Keycode key) {
  if (key == SDLK_L || key == SDLK_J || key == SDLK_H || key == SDLK_K) {
    return true;
  }
  return false;
}

void Check_numeric(SDL_Keycode key, AppState *appstate) {
  if (Is_numeric(key)) {
    int key_to_num = key - 48;
    appstate->motionState.count = appstate->motionState.count * 10 + key_to_num;
    appstate->motionCursor.tileBuffer->tileCount = appstate->motionState.count;
  }
}

bool Is_numeric(SDL_Keycode key) {
  if (key >= SDLK_0 && key <= SDLK_9) {
    return true;
  }
  return false;
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
  if (appstate->motionState.ctrl_w) {
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
  } else if (appstate->motionState.yank) {
    switch (key) {
    case SDLK_Y: {
      printf("THIS IS WHERE YANKING A LINE LOGIC GOES\n");
      break;
    }
    case SDLK_H:
    case SDLK_J:
    case SDLK_L: {
      if (appstate->motionState.count > 0) {
        Copy_direction(key, appstate);
      }
      break;
    }
    default: {
      if (key >= SDLK_0 && key <= SDLK_9) {
        break;
      }
      int camxmod = appstate->cameraPanel.x;
      int camymod = appstate->cameraPanel.y;
      appstate->motionCursor.tileBuffer[0].buffX =
          appstate->motionCursor.x + camxmod;
      appstate->motionCursor.tileBuffer[0].buffY =
          appstate->motionCursor.y + camymod;
      Reset_motion_state(appstate);
      break;
    }
    }
  } else if (appstate->motionState.pasting == true) {
    switch (key) {
    case SDLK_L: {
      Paste_tile_contents(appstate, true);
      Reset_motion_state(appstate);
    }
    }
  } else {
    Reset_motion_state(appstate);
  }
}

void Copy_direction(SDL_Keycode key, AppState *appstate) {
  switch (key) {
  case SDLK_L: {
    int temp_count = appstate->motionState.count;
    printf("THE TEMP COUNT %d\n", temp_count);
    for (int i = 0; i < temp_count; i++) {
      Copy_tile_to_cursor(i, 0, i, appstate);
    }
    break;
  }
  case SDLK_H: {
    int temp_count = appstate->motionState.count;
    for (int i = 0; i < temp_count; i++) {
      int adjusted_tile = temp_count - (i + 1);
      Copy_tile_to_cursor(-adjusted_tile, 0, i, appstate);
    }
    break;
  }
  case SDLK_J: {
    int temp_count = appstate->motionState.count;
    for (int i = 0; i < temp_count; i++) {
      Copy_tile_to_cursor(0, i, i, appstate);
    }
    break;
  }
  }
}

void Copy_tile_to_cursor(int x, int y, int i, AppState *appstate) {
  int camXmod = appstate->cameraPanel.x + x;
  int camYmod = appstate->cameraPanel.y + y;
  if (!appstate->motionCursor.mode) {
    camXmod = appstate->camera.x + x;
    camYmod = appstate->camera.y + y;
  }
  appstate->motionCursor.tileBuffer[i].buffX =
      appstate->motionCursor.x + camXmod;
  appstate->motionCursor.tileBuffer[i].buffY =
      appstate->motionCursor.y + camYmod;
  appstate->motionCursor.tileBuffer[i].validTexture = true;
  Reset_motion_state(appstate);
}

void Motion_non_state_event_handler(SDL_Keycode key, AppState *appstate) {
  switch (key) {
  case SDLK_P: {
    if (!appstate->motionCursor.mode) {
      appstate->motionState.pasting = true;
      appstate->motionState.modified = true;
    }
    break;
  }
  }
}

void Paste_tile_contents(AppState *appstate, bool x_row) {
  AppState *as = (AppState *)appstate;
  int temp_count = appstate->motionCursor.tileBuffer->tileCount;
  int x_mod = 0;
  int y_mod = 0;
  for (int i = 0; i < temp_count; i++) {
    if (as->motionCursor.tileBuffer[i].validTexture) {
      if (as->motionCursor.x + i < MAP_WIDTH) {
        printf("The xmod is %d\n", x_mod);
        as->grid[as->motionCursor.x + x_mod][as->motionCursor.y + y_mod]
            .textureLoc.buffX = as->motionCursor.tileBuffer[i].buffX;
        as->grid[as->motionCursor.x + x_mod][as->motionCursor.y + y_mod]
            .textureLoc.buffY = as->motionCursor.tileBuffer[i].buffY;
        as->grid[as->motionCursor.x + x_mod][as->motionCursor.y + y_mod]
            .textureLoc.validTexture = true;
        if (x_row) {
          x_mod++;
        } else {
          y_mod++;
        }
      }
    }
  }
}

void Reset_motion_state(AppState *appstate) {
  if (appstate->motionState.modified) {
    appstate->motionState.ctrl_w = false;
    appstate->motionState.yank = false;
    appstate->motionState.pasting = false;
    appstate->motionState.modified = false;
    appstate->motionState.count = 0;
  }
}

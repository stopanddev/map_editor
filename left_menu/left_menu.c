#include "left_menu.h"
#include <SDL3/SDL_stdinc.h>

void draw_back_panel(SDL_Renderer *renderer, AppState *appstate) {
  SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
  SDL_FRect square = {0, 0, MENU_WIDTH, 768};

  SDL_RenderFillRect(renderer, &square);
}

void handle_left_panel_camera_input(AppState *appstate, SDL_Keycode key) {
  SDL_Keymod modState = SDL_GetModState();

  if (appstate->motionCursor.mode) {
    if (modState == SDL_KMOD_LCTRL && key == SDLK_H) {
      appstate->cameraPanel.x =
          fmax(0, fmin(appstate->cameraPanel.x -= 1, MENU_WIDTH));
    }
    if (modState == SDL_KMOD_LCTRL && key == SDLK_L) {
      appstate->cameraPanel.x =
          fmax(0, fmin(appstate->cameraPanel.x += 1, MENU_WIDTH));
    }
    if (modState == SDL_KMOD_LCTRL && key == SDLK_K) {
      appstate->camera.y =
          fmax(0, fmin(appstate->cameraPanel.y -= 1, GAME_HEIGHT));
    }
    if (modState == SDL_KMOD_LCTRL && key == SDLK_J) {
      appstate->camera.y =
          fmax(0, fmin(appstate->cameraPanel.y += 1, GAME_HEIGHT));
    }
  }
}

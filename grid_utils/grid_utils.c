#include "grid_utils.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_stdinc.h>

void init_camera(Camera *camera, int width, int height) {
  camera->x = MENU_WIDTH;
  camera->y = 0;
  camera->width = width;
  camera->height = height;
}

// Handle input for the camera (scrolling)
void handle_camera_input(AppState *appstate, SDL_Keycode key) {
  SDL_Keymod modState = SDL_GetModState();
  if (!appstate->motionCursor.mode) {
    if (modState & SDL_KMOD_CTRL && key == SDLK_LEFT) {
      appstate->camera.x -= TILE_PIXEL;
    }
    if (modState & SDL_KMOD_CTRL && key == SDLK_RIGHT) {
      appstate->camera.x += TILE_PIXEL;
    }
    if (modState & SDL_KMOD_CTRL && key == SDLK_UP) {
      appstate->camera.y -= TILE_PIXEL;
    }
    if (modState & SDL_KMOD_CTRL && key == SDLK_DOWN) {
      appstate->camera.y += TILE_PIXEL;
    }
  }
}

void draw_grid(SDL_Renderer *renderer, AppState *appstate) {

  for (int r = 0; r <= MAP_HEIGHT; r++) {
    float y = SDL_floor((r * TILE_PIXEL) + (r * GAP));
    for (int c = 0; c <= MAP_WIDTH; c++) {
      float x = SDL_floor(MENU_WIDTH + (c * TILE_PIXEL) + (c * GAP));
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      float screen_x = x - (appstate->camera.x + (GAP * c));
      float screen_y = y - (appstate->camera.y + (GAP * r));
      if (screen_x + TILE_PIXEL > 0 && screen_x < appstate->camera.width &&
          screen_y + TILE_PIXEL > 0 && screen_y < appstate->camera.height) {
        SDL_FRect square = {x, y, TILE_PIXEL, TILE_PIXEL};
        SDL_RenderFillRect(renderer, &square);
      }
    }
  }
}

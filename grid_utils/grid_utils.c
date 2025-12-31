#include "grid_utils.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_stdinc.h>

void Init_camera(Camera *camera, int width, int height) {
  camera->x = MENU_WIDTH;
  camera->y = 0;
  camera->width = width;
  camera->height = height;
}

// Handle input for the camera (scrolling)
void Handle_camera_input(AppState *appstate, SDL_Keycode key) {
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

void Draw_grid(SDL_Renderer *renderer, AppState *appstate) {
  AppState *as = (AppState *)appstate;
  for (int r = 0; r < MAP_HEIGHT; r++) {
    float y = SDL_floor((r * TILE_PIXEL) + (r * GAP));
    for (int c = 0; c < MAP_WIDTH; c++) {
      float x = SDL_floor(MENU_WIDTH + (c * TILE_PIXEL) + (c * GAP));
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      int camModx = (appstate->camera.x - 224) + x;
      int camMody = (appstate->camera.y) + y;
      if (as->grid[c][r].textureLoc.validTexture == true) {
        SDL_FRect src = {(as->grid[c][r].textureLoc.buffX) * 32,
                         (as->grid[c][r].textureLoc.buffY) * 32, 32, 32};
        SDL_FRect dest = {camModx, camMody, TILE_PIXEL, TILE_PIXEL};
        SDL_RenderTexture(renderer, as->tileMapTexture.texture, &src, &dest);
      } else {
        SDL_FRect square = {camModx, camMody, TILE_PIXEL, TILE_PIXEL};
        SDL_RenderFillRect(renderer, &square);
      }
    }
  }
}

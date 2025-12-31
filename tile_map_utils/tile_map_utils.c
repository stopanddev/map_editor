#include "tile_map_utils.h"
#include <SDL3/SDL_oldnames.h>

SDL_AppResult Tile_map_texture_init(AppState *appstate) {
  SDL_Surface *surface = NULL;
  char *png_path = NULL;
  SDL_asprintf(
      &png_path,
      "assets/TX Struct.png"); /* allocate a string of the full file path */

  surface = SDL_LoadPNG(png_path);
  if (!surface) {
    SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_free(png_path); /* done with this, the file is loaded. */
  appstate->tileMapTexture.texture_width = surface->w;
  appstate->tileMapTexture.texture_height = surface->h;
  appstate->tileMapTexture.texture =
      SDL_CreateTextureFromSurface(appstate->renderer, surface);
  if (!appstate->tileMapTexture.texture) {
    SDL_Log("Couldn't create static texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
  SDL_DestroySurface(surface);
}
// Render left side panels
void Render_panel_tiles(AppState *appstate) {
  AppState *as = (AppState *)appstate;
  float twidth, theight;
  SDL_GetTextureSize(as->tileMapTexture.texture, &twidth, &theight);
  int camxmod = as->cameraPanel.x;
  int camymod = as->cameraPanel.y;
  for (int x = 0; x < MENU_COLUMNS; x++) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
      SDL_FRect src = {(x + camxmod) * 32, (y + camymod) * 32, 32, 32};
      SDL_FRect dest = {x * 32, y * 32, 32, 32};
      SDL_RenderTexture(as->renderer, as->tileMapTexture.texture, &src, &dest);
    }
  }
}

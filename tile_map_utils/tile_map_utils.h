#ifndef tile_map_utils_h
#define tile_map_utils_h
#include "../configs/init.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
SDL_AppResult Tile_map_texture_init(AppState *appstate);
void Render_panel_tiles(AppState *appstate);
#endif

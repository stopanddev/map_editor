#ifndef init_h
#define init_h

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <stdio.h>

#define STEP_RATE_MILLISECONDS 60
#define GAME_WIDTH 1024
#define GAME_HEIGHT 768
#define TILE_PIXEL 32
#define MENU_WIDTH 160
#define GAP 1.0f
// (1024 - 160) / 33
#define MAP_WIDTH 25
#define MAP_HEIGHT 21
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef struct {
  int textureId;
  int collidable;
} Tile;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 last_step;
  Tile grid[MAP_WIDTH][MAP_HEIGHT];
  SDL_Cursor *cursor;
  Uint32 prevMouseButton;
} AppState;

SDL_AppResult Init(AppState *state, int argc, char *argv[]);

#endif

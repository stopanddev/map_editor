#ifndef init_h
#define init_h

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <stdio.h>

#define BLOCK_SIZE_IN_PIXELS 16
#define STEP_RATE_MILLISECONDS 60
#define GAME_WIDTH 84
#define GAME_HEIGHT 76
#define SDL_WINDOW_WIDTH (BLOCK_SIZE_IN_PIXELS * GAME_WIDTH)
#define SDL_WINDOW_HEIGHT (BLOCK_SIZE_IN_PIXELS * GAME_HEIGHT)
#define CELL_SIZE (BLOCK_SIZE_IN_PIXELS + 1)
#define COLS ((SDL_WINDOW_WIDTH - 1) / CELL_SIZE)
#define ROWS ((SDL_WINDOW_HEIGHT - 1) / CELL_SIZE)

static SDL_Joystick *joystick = NULL;

typedef struct {
  int textureId;
  int collidable;
} Tile;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 last_step;
  Tile grid[ROWS][COLS];
  SDL_Cursor *cursor;
  Uint32 prevMouseButton;
} AppState;

SDL_AppResult Init(AppState *state, int argc, char *argv[]);

#endif

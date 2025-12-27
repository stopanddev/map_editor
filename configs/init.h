#ifndef init_h
#define init_h

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <stdio.h>
#define STEP_RATE_MILLISECONDS 10
#define GAME_WIDTH 1024
#define GAME_HEIGHT 768
#define TILE_PIXEL 32
#define MENU_WIDTH 224
#define MENU_COLUMNS 7
#define GAP 1.0f
// (1024 - 160) / 33
#define MAP_WIDTH 25
#define MAP_HEIGHT 22
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef struct {
  int textureId;
  int collidable;
} Tile;

typedef struct Camera {
  int x, y;
  int width, height;
} Camera;

typedef struct MotionCursor {
  int x, y, gridx, gridy, panelx, panely;
  bool mode;
} MotionCursor;

typedef struct MotionState {
  bool ctrl_w;
} MotionState;

typedef struct TileMapTexture {
  int texture_width, texture_height;
  SDL_Texture *texture;
  // SDL_FRect destSquare;
  // SDL_FRect sourceSquare;
} TileMapTexture;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 last_step;
  Tile grid[MAP_WIDTH][MAP_HEIGHT];
  SDL_Cursor *cursor;
  Uint32 prevMouseButton;
  Camera camera;
  Camera cameraPanel;
  MotionCursor motionCursor;
  MotionState motionState;
  TileMapTexture tileMapTexture;
} AppState;

SDL_AppResult Init(AppState *state, int argc, char *argv[]);

#endif

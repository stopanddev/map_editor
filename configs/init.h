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
#define MAP_WIDTH 25
#define MAP_HEIGHT 23
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

typedef struct Camera {
  int x, y;
  int width, height;
} Camera;

typedef struct {
  int buffX, buffY;
  bool validTexture;
} TileBuffer;

typedef struct {
  TileBuffer textureLoc;
  int collidable;
} Tile;

typedef struct MotionCursor {
  int x, y, gridx, gridy, panelx, panely;
  bool mode;
  TileBuffer tileBuffer;
} MotionCursor;

typedef struct MotionState {
  bool modified;
  bool ctrl_w;
  bool yank;
} MotionState;

typedef struct TileMapTexture {
  int texture_width, texture_height;
  SDL_Texture *texture;
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
void Init_motion_state(AppState *appstate);
#endif

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
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

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  size_t i;

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
    SDL_Log("Couldn't initalize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
  if (!as) {
    return SDL_APP_FAILURE;
  }

  *appstate = as;

  if (!SDL_CreateWindowAndRenderer("Testing", SDL_WINDOW_WIDTH,
                                   SDL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                   &as->window, &as->renderer)) {
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(as->renderer, SDL_WINDOW_WIDTH,
                                   SDL_WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  as->last_step = SDL_GetTicks();
  as->cursor = SDL_GetDefaultCursor();

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      as->grid[r][c].textureId = 0;
      as->grid[r][c].collidable = 0;
    }
  }

  return SDL_APP_CONTINUE;
}

void LoadMap(AppState *appstate) {
  FILE *f = fopen("firstMap", "r");
  if (!f) {
    printf("Failed to open file %s\n", "firstMap");
    return;
  }

  char buffer[4096]; // read chunks of the file
  size_t bytesRead;
  char *ptr;

  while ((bytesRead = fread(buffer, 1, sizeof(buffer) - 1, f)) > 0) {
    buffer[bytesRead] = '\0'; // null-terminate
    ptr = buffer;

    int tx, ty, textureId, collidable;
    // Scan for tile objects in the text
    while ((ptr = strstr(ptr, "\"x\":")) != NULL) {
      if (sscanf(ptr,
                 "\"x\": %d, \"y\": %d, \"textureId\": %d, \"collidable\": %d",
                 &tx, &ty, &textureId, &collidable) == 4) {
        if (tx >= 0 && tx < GAME_WIDTH && ty >= 0 && ty < GAME_HEIGHT) {
          appstate->grid[tx][ty].textureId = textureId;
          appstate->grid[tx][ty].collidable = collidable;
        }
      }
      ptr++; // advance to avoid infinite loop
    }
  }

  fclose(f);
}

void SaveMap(AppState *appstate) {
  FILE *f = fopen("firstMap", "w");
  if (!f)
    return;

  fprintf(f, "{\n \"First Map\": [\n");
  for (int r = 0; r < ROWS; r++) {
    fprintf(f, "    [");
    for (int c = 0; c < COLS; c++) {
      Tile t = appstate->grid[r][c];
      fprintf(f, "{\"x\":%d, \"y\":%d, \"textureId\":%d, \"collidable\":%d}", r,
              c, t.textureId, t.collidable);
      if (c < COLS - 1)
        fprintf(f, ",");
    }
    fprintf(f, "]");
    if (r < ROWS - 1)
      fprintf(f, ",");
  }
  fprintf(f, "  ]\n}\n");
  fclose(f);
}

void DrawGrid(SDL_Renderer *renderer, AppState *appstate) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white blocks

  int num_cols = (SDL_WINDOW_WIDTH - 1) / (BLOCK_SIZE_IN_PIXELS + 1);
  int num_rows = (SDL_WINDOW_HEIGHT - 1) / (BLOCK_SIZE_IN_PIXELS + 1);
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      float x = 1.0f + c * (BLOCK_SIZE_IN_PIXELS + 1);
      float y = 1.0f + r * (BLOCK_SIZE_IN_PIXELS + 1);

      Tile t = appstate->grid[r][c];

      SDL_SetRenderDrawColor(renderer, t.collidable ? 255 : 255,
                             t.collidable ? 0 : 255, t.collidable ? 0 : 255,
                             255);

      SDL_FRect rect = {x, y, 16, 16};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void HandleMouseClick(SDL_Renderer *renderer, AppState *appstate,
                      SDL_FPoint mousePoint, Uint32 mbutton) {
  if (mbutton != SDL_BUTTON_LEFT || mbutton == appstate->prevMouseButton) {
    return;
  }
  float mx = mousePoint.x - 1.0f;
  float my = mousePoint.y - 1.0f;
  if (my > SDL_WINDOW_HEIGHT || my < 0 || mx > SDL_WINDOW_WIDTH || mx < 0) {
    return;
  }

  int col = (int)(mx / (BLOCK_SIZE_IN_PIXELS + 1));
  int row = (int)(my / (BLOCK_SIZE_IN_PIXELS + 1));

  float remX = fmodf(mx, BLOCK_SIZE_IN_PIXELS + 1);
  float remY = fmodf(my, BLOCK_SIZE_IN_PIXELS + 1);

  if (remX < BLOCK_SIZE_IN_PIXELS && remY < BLOCK_SIZE_IN_PIXELS) {
    // Mouse is inside square
    Tile *t = &appstate->grid[row][col];
    t->collidable = !t->collidable;
  }
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *as = (AppState *)appstate;
  const Uint64 now = SDL_GetTicks();
  SDL_FRect r;
  unsigned i;
  unsigned j;
  int ct;
  float mouseX, mouseY;

  Uint32 mbutton = SDL_GetMouseState(&mouseX, &mouseY);
  SDL_FPoint mousePoint = {mouseX, mouseY};
  HandleMouseClick(as->renderer, as, mousePoint, mbutton);
  as->prevMouseButton = mbutton;
  // run game logic if we're at or past the time to run it.
  // if we're _really_ behind the time to run it, run it
  // several times.
  while ((now - as->last_step) >= STEP_RATE_MILLISECONDS) {
    as->last_step += STEP_RATE_MILLISECONDS;
  }
  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderClear(as->renderer);

  DrawGrid(as->renderer, as);
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_UP: {
    if (event->key.key == SDLK_S) {
      SaveMap(appstate);
    }
    if (event->key.key == SDLK_L) {
      LoadMap(appstate);
    }
    break;
  }
  defualt:
    break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (joystick) {
    SDL_CloseJoystick(joystick);
  }
  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
};

#include "system_utils.h"

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

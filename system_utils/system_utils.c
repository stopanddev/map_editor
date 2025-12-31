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
          // appstate->grid[tx][ty].textureId = textureId;
          // appstate->grid[tx][ty].collidable = collidable;
        }
      }
      ptr++; // advance to avoid infinite loop
    }
  }

  fclose(f);
}

void SaveMap(AppState *appstate) {
  FILE *f = fopen("firstMap.json", "w");
  if (!f)
    return;

  fprintf(f, "{\n  \"First Map\": [\n");
  for (int r = 0; r < GAME_HEIGHT; r++) {
    fprintf(f, "    [");
    for (int c = 0; c < GAME_WIDTH - 40; c++) { // only map, skip menu
      Tile t = appstate->grid[r][c];
      fprintf(f, "{\"x\":%d, \"y\":%d, \"textureId\":%d, \"collidable\":%d}", r,
              c, t.textureLoc.buffX, t.collidable);
      if (c < (GAME_WIDTH - 41))
        fprintf(f, ",");
    }
    fprintf(f, "]");
    if (r < GAME_HEIGHT - 1)
      fprintf(f, ",");
    fprintf(f, "\n");
  }
  fprintf(f, "  ]\n}\n");
  fclose(f);
}

void HandleMouseClick(AppState *appstate, SDL_FPoint mousePoint,
                      Uint32 mbutton) {
  // Only handle left click, ignore repeats
  if (mbutton != SDL_BUTTON_LEFT || mbutton == appstate->prevMouseButton) {
    return;
  }

  // Logical coordinates (already handled by SDL with logical scaling)
  int col = (int)mousePoint.x; // 0..159
  int row = (int)mousePoint.y; // 0..89

  // Check bounds
  if (row < 0 || row >= GAME_HEIGHT || col < 0 || col >= GAME_WIDTH) {
    return;
  }

  // If clicked inside menu, ignore
  if (col < 40) {
    return;
  }

  // Adjust for map columns starting after menu
  int mapCol = col - 40;

  // Toggle tile
  Tile *t = &appstate->grid[row][mapCol];
  t->collidable = !t->collidable;
}

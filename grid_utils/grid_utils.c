#include "grid_utils.h"

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

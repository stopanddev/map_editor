#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include "configs/init.h"
#include "controls/controls.h"
#include "grid_utils/grid_utils.h"
#include "left_menu/left_menu.h"
#include "motion_utils/motion_utils.h"
#include "system_utils/system_utils.h"
#include "tile_map_utils/tile_map_utils.h"
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  freopen("log.txt", "w", stdout);
  AppState *state = SDL_calloc(1, sizeof(AppState));
  if (!state) {
    return SDL_APP_FAILURE;
  }
  Init_camera(&state->camera, GAME_WIDTH - MENU_WIDTH, GAME_HEIGHT);
  Init_motion_cursor(&state->motionCursor);
  *appstate = state;
  if (Init(state, argc, argv) != SDL_APP_CONTINUE) {
    return SDL_APP_FAILURE;
  }
  if (Tile_map_texture_init(*appstate) != SDL_APP_CONTINUE) {
    printf("Failed to initiate texture\n");
    return SDL_APP_FAILURE;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *as = (AppState *)appstate;
  const Uint64 now = SDL_GetTicks();
  float mouseX, mouseY;

  SDL_FRect dst_rect;

  Uint32 mbutton = SDL_GetMouseState(&mouseX, &mouseY);
  SDL_FPoint mousePoint = {mouseX, mouseY};
  HandleMouseClick(as, mousePoint, mbutton);
  as->prevMouseButton = mbutton;
  // run game logic if we're at or past the time to run it.
  while ((now - as->last_step) >= STEP_RATE_MILLISECONDS) {
    as->last_step += STEP_RATE_MILLISECONDS;
  }
  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderClear(as->renderer);

  Draw_grid(as->renderer, as);
  Draw_back_panel(as->renderer, as);

  Render_panel_tiles(appstate);
  Draw_motion_mode_handler(as->renderer, as);
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *as = (AppState *)appstate;
  SDL_AppResult res = Handle_input(event, appstate);
  return res;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_DestroyTexture(as->tileMapTexture.texture);
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
};

#ifndef system_utils_h
#define system_utils_h
#include "../configs/init.h"
#include <math.h>

void LoadMap(AppState *appstate);
void SaveMap(AppState *appstate);

void HandleMouseClick(SDL_Renderer *renderer, AppState *appstate,
                      SDL_FPoint mousePoint, Uint32 mbutton);
#endif

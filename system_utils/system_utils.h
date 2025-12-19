#ifndef system_utils_h
#define system_utils_h
#include "../configs/init.h"
#include <math.h>

void LoadMap(AppState *appstate);
void SaveMap(AppState *appstate);

void HandleMouseClick(AppState *appstate, SDL_FPoint mousePoint,
                      Uint32 mbutton);
static inline float SnapX(float logicalX, float scaleX) {
  return floorf(logicalX * scaleX) / scaleX;
}

static inline float SnapY(float logicalY, float scaleY) {
  return floorf(logicalY * scaleY) / scaleY;
}
#endif

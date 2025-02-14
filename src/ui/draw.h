#ifndef UI_DRAW_H
#define UI_DRAW_H

#include "raylib.h"
#include "state.h"

#define PADDING 10
#define TEXT_SIZE 20
#define ITEM_HEIGHT 45
#define ACTIVE_COLOR (Color){ 48, 48, 48, 255 }
#define INACTIVE_COLOR (Color){ 32, 32, 32, 255 }
#define TEXT_COLOR RAYWHITE
#define CURSOR_BLINK_RATE 0.5f

void DrawInputBox(UIState* state, Rectangle bounds, char* text, int maxSize, bool captureInput);
void DrawListView(UIState* state, Rectangle bounds, const char** items, int count, int* selectedIndex);

#endif // UI_DRAW_H

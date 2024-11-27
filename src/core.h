#ifndef APP_CORE_H
#define APP_CORE_H

#include "events.h"
#include "modes.h"
#include "utils.h"

#include "raylib.h"

#include <string.h>
#include <stdlib.h>

#define INPUT_TEXT_MAX_SIZE 256

typedef enum {
    F_TEXTBOX = -1,
    F_LIST
} Focusable;

typedef struct Focus {
    int index;
    Focusable object;
} Focus;

typedef struct {
    char inputText[INPUT_TEXT_MAX_SIZE];
    char **listItems;
    int listCount;
    int listScrollIndex;
    Focus focus;
} AppState;

void InitAppState(AppState* state);
void CleanupAppState(AppState* state);

void HandleAppEvent(AppState* state, ModeManager* ModeManager, AppEvent event);

int PerformSearch(const char* query, char*** result_list, int* result_count);

#endif // APP_CORE_H
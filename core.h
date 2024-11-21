#ifndef APP_CORE_H
#define APP_CORE_H

#include "events.h"
#include "listview.h"

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
    char input_text[INPUT_TEXT_MAX_SIZE];
    char **list_items;
    int list_count;
    int list_scroll_index;
    Focus focus;
} AppState;

void app_state_init(AppState* state);
void app_state_cleanup(AppState* state);

void handle_app_event(AppState* state, AppEvent* event);

int perform_search(const char* query, char*** result_list, int* result_count);

#endif // APP_CORE_H
#include "input/definitions.h"
#include "callbacks/events.h"
#include "input/keybinds.h"
#include "lua.h"
#include "state.h"

#include "raylib.h"
#include <string.h>

void ProcessInput(State* state, EventQueue* eventQueue, lua_State* L) {
    // NOTE: Keybinds is the newer input system, the rest after this call
    // is just to add to input when focused, as that is not supported by keybinds
    // TODO: Move the input processing to the inputbox itself when doing a rewrite of 
    // the widgets
    ProcessKeybinds(state, &state->keybindStore, L);

    // Process text input when search bar is selected (focus = -1)
    if (state->focus.index == -1 || state->focus.index >= 0) {
        // Handle character input
        int key = GetCharPressed();
        while (key > 0) {
            unsigned long len = strlen(state->input);
            if (len < sizeof(state->input) - 1) {
                state->input[len] = (char)key;
                state->input[len + 1] = '\0';
            }

            key = GetCharPressed();
        }

        // Handle backspace separately
        if (IsKeyPressed(KEY_BACKSPACE) && state->focus.index == -1) {
            TraceLog(LOG_DEBUG, "Backspace detected.");
            unsigned long len = strlen(state->input);
            if (len > 0) {
                state->input[len - 1] = '\0';
            }

        }

        // Trigger search by enter
        if (state->focus.index < 0 && IsKeyPressed(KEY_ENTER)) {
            TraceLog(LOG_DEBUG, "Search key pressed.");

            Event searchEvent = {
                .type = EVENT_SEARCH_TRIGGERED,
            };
            EventQueuePush(eventQueue, searchEvent);
        }
    }
}

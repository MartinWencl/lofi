#include "input/definitions.h"
#include "callbacks/events.h"
#include "state.h"
#include <string.h>

void ProcessInput(State* state, EventQueue* eventQueue) {
    // Keyboard focus changes
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP) || GetMouseWheelMove() != 0) {
        TraceLog(LOG_DEBUG, "Up/Down key pressed.");
        TraceLog(LOG_DEBUG, "Focus: %d", state->focus.index);

        int newIndex = state->focus.index;

        if (IsKeyPressed(KEY_DOWN)) newIndex++;
        else if (IsKeyPressed(KEY_UP)) newIndex--;

        // Handle scroll wheel
        newIndex += (int)GetMouseWheelMove();

        // Bound checking
        if (newIndex < -1) newIndex = -1;
        else if (newIndex >= state->listCount)
            newIndex = state->listCount - 1;

        // Scroll event
        if (GetMouseWheelMove() != 0) {
            Event scrollEvent = {
                .type = EVENT_SCROLL,
                .data.scroll.scrollAmount = (int)GetMouseWheelMove()
            };
            EventQueuePush(eventQueue, scrollEvent);
        }

        // Focus change event
        Event focus_event = {
            .type = EVENT_FOCUS_CHANGED,
            .data.focus.index = newIndex,
        };
        EventQueuePush(eventQueue, focus_event);
    }

    // Item selection
    if (state->focus.index >= 0 && IsKeyPressed(KEY_ENTER)) {
        TraceLog(LOG_DEBUG, "Selection key pressed.");

        Event selectEvent = {
            .type = EVENT_ITEM_SELECTED,
            .data.selection.selectedIndex = state->focus.index
        };
        EventQueuePush(eventQueue, selectEvent);
    }

    // Trigger search by enter
    if (state->focus.index < 0 && IsKeyPressed(KEY_ENTER)) {
        TraceLog(LOG_DEBUG, "Search key pressed.");

        Event searchEvent = {
            .type = EVENT_SEARCH_TRIGGERED,
        };
        EventQueuePush(eventQueue, searchEvent);
    }


        if (state->focus.index == -1) {
        // Handle character input
        int key = GetCharPressed();
        while (key > 0) {
            // Add character to input
            int len = strlen(state->input);
            if (len < sizeof(state->input) - 1) {
                state->input[len] = (char)key;
                state->input[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        // Handle backspace separately
        if (IsKeyPressed(KEY_BACKSPACE)) {
            TraceLog(LOG_DEBUG, "Backspace detected.");
            int len = strlen(state->input);
            if (len > 0) {
                state->input[len - 1] = '\0';
            }
        }
    }
}

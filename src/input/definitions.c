#include "input/definitions.h"
#include "callbacks/events.h"
#include "state.h"

// original function
void ProcessInput(State* state, EventQueue* eventQueue) {
    // Keyboard focus changes
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP) || GetMouseWheelMove() != 0) {
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
            .data.focus.objectType = (newIndex == -1) ? F_TEXTBOX : F_LIST
        };
        EventQueuePush(eventQueue, focus_event);
    }

    // Item selection
    if (state->focus.object == F_LIST && IsKeyPressed(KEY_ENTER)) {
        Event selectEvent = {
            .type = EVENT_ITEM_SELECTED,
            .data.selection.selectedIndex = state->focus.index
        };
        EventQueuePush(eventQueue, selectEvent);
    }
    
    // Trigger search by enter
    if (state->focus.object == F_TEXTBOX && IsKeyPressed(KEY_ENTER)) {
        Event searchEvent = {
            .type = EVENT_SEARCH_TRIGGERED,
        };
        EventQueuePush(eventQueue, searchEvent);
    }
}

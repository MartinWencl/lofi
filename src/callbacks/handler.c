#include "callbacks/handler.h"
#include "utils.h"

void HandleEvent(State* state, ModeManager* modeManager, Event event) {
    switch (event.type) {
        case EVENT_SEARCH_TRIGGERED:
            // Clear previous results
            ClearListViewExList(&state->list, &state->listCount);
            
            // NOTE: Temporary search that lists files in current directory
            Search(state->input, &state->list, &state->listCount);

            // call lua callback
            // DispatchLuaEvent(state, modeManager, event.type);
            
            // Reset focus if results found
            if (state->listCount > 0) {
                state->focus.index = 0;
            }
            break;
        
        case EVENT_FOCUS_CHANGED:
            state->focus.index = event.data.focus.index;
            TraceLog(LOG_DEBUG, "Focus changed, new: %d.", state->focus.index);
            break;
        
        case EVENT_ITEM_SELECTED:
            TraceLog(LOG_INFO, "Selected item at index: %d", event.data.selection.selectedIndex);
            // DispatchLuaEvent(state, modeManager, event.type);
            break;

        default:
            break;
    }
}

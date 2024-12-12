#include "callbacks/handler.h"
#include "utils.h"

void HandleEvent(State* state, ModeManager* modeManager, Event event) {
    switch (event.type) {
        case EVENT_SEARCH_TRIGGERED:
            // Clear previous results
            ClearListViewExList(&state->list, &state->listCount);
            
            // Perform search
            //PerformSearch(state->inputText, &state->list, &state->listCount);

            // call lua callback
            DispatchLuaEvent(state, modeManager, event.type);
            
            // Reset focus if results found
            if (state->listCount > 0) {
                state->focus.index = 0;
                state->focus.object = F_LIST;
            }
            break;
        
        case EVENT_FOCUS_CHANGED:
            state->focus.index = event.data.focus.index;
            state->focus.object = event.data.focus.objectType;
            break;
        
        case EVENT_ITEM_SELECTED:
            TraceLog(LOG_INFO, "Selected item at index: %d", event.data.selection.selectedIndex);
            DispatchLuaEvent(state, modeManager, event.type);
            break;
        
        default:
            break;
    }
}

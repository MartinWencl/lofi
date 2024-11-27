#include "core.h"
#include "lua_core.h"

#include <string.h>
#include <stdlib.h>

void InitAppState(AppState* state) {
    memset(state->inputText, 0, sizeof(state->inputText));
    state->listItems = NULL;
    state->listCount = 0;
    state->listScrollIndex = 0;
    state->focus.index = -1;
    state->focus.object = F_TEXTBOX;
    
    // Initial search to populate list
    //PerformSearch("", &state->listItems, &state->listCount);
    TraceLog(LOG_DEBUG, "STATE: Initialized the general app state.");
}

void CleanupAppState(AppState* state) {
    ClearListViewExList(&state->listItems, &state->listCount);
}

void HandleAppEvent(AppState* state, ModeManager* modeManager, AppEvent event) {
    switch (event.type) {
        case EVENT_SEARCH_TRIGGERED:
            // Clear previous results
            ClearListViewExList(&state->listItems, &state->listCount);
            
            // Perform search
            //PerformSearch(state->inputText, &state->listItems, &state->listCount);

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
        
        case EVENT_SCROLL:
            // Adjust scroll index based on wheel movement
            state->listScrollIndex += event.data.scroll.scrollAmount;
            
            // Ensure scroll index remains in bounds
            if (state->listScrollIndex < 0) state->listScrollIndex = 0;
            else if (state->listScrollIndex >= state->listCount) 
                state->listScrollIndex = state->listCount - 1;
            break;
        
        default:
            break;
    }
}

// Placeholder implementation - replace with actual search logic
int PerformSearch(const char* query, char*** resultList, int* resultCount) {
    // This would typically be replaced with your actual search implementation
    Search(query, resultList, resultCount);
    return 0;
}
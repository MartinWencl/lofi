#include "callbacks/handler.h"
#include "utils.h"

void HandleEvent(State* state, ModeManager* modeManager, Event event) {
    switch (event.type) {
        case EVENT_SEARCH_TRIGGERED:
            // Clear previous results
            ClearListViewExList(&state->list, &state->listCount);
            
            // Perform search
            //PerformSearch(state->inputText, &state->list, &state->listCount);

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

// handler.c
// #include "callbacks/handler.h"
//
// void HandleEvent(State* state, ModeManager* modeManager, Event event) {
//     switch (event.type) {
//         case EVENT_SEARCH_TRIGGERED:
//             DispatchLuaEvent(state, modeManager, EVENT_SEARCH_TRIGGERED);
//             break;
//             
//         case EVENT_FOCUS_CHANGED:
//             if (state->listCount > 0) {
//                 state->focus.index += event.data;
//                 if (state->focus.index >= state->listCount) 
//                     state->focus.index = 0;
//                 if (state->focus.index < 0) 
//                     state->focus.index = state->listCount - 1;
//                 
//                 DispatchLuaEvent(state, modeManager, EVENT_FOCUS_CHANGED);
//             }
//             break;
//             
//         case EVENT_ITEM_SELECTED:
//             if (state->listCount > 0 && state->focus.index >= 0) {
//                 DispatchLuaEvent(state, modeManager, EVENT_ITEM_SELECTED);
//             }
//             break;
//             
//         case EVENT_EXIT:
//             CloseWindow();
//             break;
//     }
// }

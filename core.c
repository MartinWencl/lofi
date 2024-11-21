#include "core.h"
#include "utils.h"
#include "events.h"
#include <string.h>
#include <stdlib.h>

void app_state_init(AppState* state) {
    memset(state->input_text, 0, sizeof(state->input_text));
    state->list_items = NULL;
    state->list_count = 0;
    state->list_scroll_index = 0;
    state->focus.index = -1;
    state->focus.object = F_TEXTBOX;
    
    // Initial search to populate list
    perform_search("", &state->list_items, &state->list_count);
}

void app_state_cleanup(AppState* state) {
    ClearListViewExList(&state->list_items, &state->list_count);
}

void handle_app_event(AppState* state, AppEvent* event) {
    switch (event->type) {
        case EVENT_SEARCH_TRIGGERED:
            // Clear previous results
            ClearListViewExList(&state->list_items, &state->list_count);
            
            // Perform search
            perform_search(state->input_text, &state->list_items, &state->list_count);
            
            // Reset focus if results found
            if (state->list_count > 0) {
                state->focus.index = 0;
                state->focus.object = F_LIST;
            }
            break;
        
        case EVENT_FOCUS_CHANGED:
            state->focus.index = event->data.focus.index;
            state->focus.object = event->data.focus.object_type;
            break;
        
        case EVENT_ITEM_SELECTED:
            // Handle item selection (e.g., logging, opening)
            TraceLog(LOG_INFO, "Selected item at index: %d", event->data.selection.selected_index);
            break;
        
        case EVENT_SCROLL:
            // Adjust scroll index based on wheel movement
            state->list_scroll_index += event->data.scroll.scroll_amount;
            
            // Ensure scroll index remains in bounds
            if (state->list_scroll_index < 0) state->list_scroll_index = 0;
            else if (state->list_scroll_index >= state->list_count) 
                state->list_scroll_index = state->list_count - 1;
            break;
        
        default:
            break;
    }
}

// Placeholder implementation - replace with actual search logic
int perform_search(const char* query, char*** result_list, int* result_count) {
    // This would typically be replaced with your actual search implementation
    Search(query, result_list, result_count);
    return 0;
}
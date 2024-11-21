#include "raylib.h"
#include "core.h"
#include "events.h"
#include "listview.h"
#include "lua_core.h"

#define RAYGUI_IMPLEMENTATION 
#include "raygui.h"

#define APP_NAME "lofi"
#define INIT_WIDTH 400
#define INIT_HEIGHT 300
#define INIT_FPS 60
#define INIT_OPACITY 0.9

// Function to translate raylib inputs to app events
void process_input(AppState* state, EventQueue* event_queue) {
    // Search trigger
    // static bool prev_textbox_mode = true;
    // bool current_textbox_mode = (state->focus.object == F_TEXTBOX);
    
    // if (prev_textbox_mode && !current_textbox_mode) {
        // AppEvent search_event = {
            // .type = EVENT_SEARCH_TRIGGERED
        // };
        // event_queue_push(event_queue, search_event);
    // }
    // prev_textbox_mode = current_textbox_mode;

    // Keyboard focus changes
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP) || GetMouseWheelMove() != 0) {
        int new_index = state->focus.index;
        
        if (IsKeyPressed(KEY_DOWN)) new_index++;
        else if (IsKeyPressed(KEY_UP)) new_index--;
        
        // Handle scroll wheel
        new_index += (int)GetMouseWheelMove();
        
        // Bound checking
        if (new_index < -1) new_index = -1;
        else if (new_index >= state->list_count) 
            new_index = state->list_count - 1;
        
        // Scroll event
        if (GetMouseWheelMove() != 0) {
            AppEvent scroll_event = {
                .type = EVENT_SCROLL,
                .data.scroll.scroll_amount = (int)GetMouseWheelMove()
            };
            event_queue_push(event_queue, scroll_event);
        }
        
        // Focus change event
        AppEvent focus_event = {
            .type = EVENT_FOCUS_CHANGED,
            .data.focus.index = new_index,
            .data.focus.object_type = (new_index == -1) ? F_TEXTBOX : F_LIST
        };
        event_queue_push(event_queue, focus_event);
    }

    // Item selection
    if (state->focus.object == F_LIST && IsKeyPressed(KEY_ENTER)) {
        AppEvent select_event = {
            .type = EVENT_ITEM_SELECTED,
            .data.selection.selected_index = state->focus.index
        };
        event_queue_push(event_queue, select_event);
    }
    
    // Trigger search by enter
    if (state->focus.object == F_TEXTBOX && IsKeyPressed(KEY_ENTER)) {
        AppEvent search_event = {
            .type = EVENT_SEARCH_TRIGGERED,
        };
        event_queue_push(event_queue, search_event);
    }
}

int main(void) {
    SetTraceLogLevel(LOG_DEBUG);
    InitWindow(INIT_WIDTH, INIT_HEIGHT, APP_NAME);
    SetTargetFPS(INIT_FPS);
    SetWindowOpacity(INIT_OPACITY);
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    AppState state;
    EventQueue event_queue;
    ModeManager modeManager;
    
    app_state_init(&state);
    event_queue_init(&event_queue);
    InitModeManager(&modeManager);
    
    modeManager.luaState = InitLua("init.lua");
    if (!modeManager.luaState) {
        TraceLog(LOG_ERROR, "LUA: Failed to initialize!");
        app_state_cleanup(&state);
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {
        // Process input and generate events
        process_input(&state, &event_queue);

        // Process all pending events
        while (!event_queue_is_empty(&event_queue)) {
            AppEvent event = event_queue_pop(&event_queue);
            handle_app_event(&state, &event);
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Textbox
        bool textBoxEditMode = (state.focus.object == F_TEXTBOX);
        GuiTextBox((Rectangle){ 0, 0, INIT_WIDTH, INIT_HEIGHT * 0.2 }, 
                   state.input_text, INPUT_TEXT_MAX_SIZE, textBoxEditMode);

        // ListView
        if (state.list_count > 0) {
            int activeIndex = (state.focus.index == -1) ? 0 : state.focus.index;
            ListViewEx((Rectangle){ 0, (INIT_HEIGHT * 0.2) + 10, INIT_WIDTH, 200 }, 
                       (const char**)state.list_items, 
                       state.list_count, 
                       &activeIndex, 
                       &state.list_scroll_index);
        }

        EndDrawing();
    }

    // Cleanup
        app_state_cleanup(&state);
        CloseWindow();

    return 0;
}
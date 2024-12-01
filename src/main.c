#include "raylib.h"
#include "core.h"
#include "events.h"
#include "listview.h"
#include "lua_core.h"
#include "modes.h"
#include "startup.h"
#include "ui.h"

#define RAYGUI_IMPLEMENTATION 
#include "raygui.h"

#define APP_NAME "lofi"
#define INIT_WIDTH 400
#define INIT_HEIGHT 300
#define INIT_FPS 60
#define INIT_OPACITY 0.9

// Function to translate raylib inputs to app events
void ProcessInput(AppState* state, EventQueue* eventQueue) {
    // Search trigger
    // static bool prev_textbox_mode = true;
    // bool current_textbox_mode = (state->focus.object == F_TEXTBOX);
    
    // if (prev_textbox_mode && !current_textbox_mode) {
        // AppEvent search_event = {
            // .type = EVENT_SEARCH_TRIGGERED
        // };
        // EventQueuePush(eventQueue, search_event);
    // }
    // prev_textbox_mode = current_textbox_mode;

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
            AppEvent scrollEvent = {
                .type = EVENT_SCROLL,
                .data.scroll.scrollAmount = (int)GetMouseWheelMove()
            };
            EventQueuePush(eventQueue, scrollEvent);
        }
        
        // Focus change event
        AppEvent focus_event = {
            .type = EVENT_FOCUS_CHANGED,
            .data.focus.index = newIndex,
            .data.focus.objectType = (newIndex == -1) ? F_TEXTBOX : F_LIST
        };
        EventQueuePush(eventQueue, focus_event);
    }

    // Item selection
    if (state->focus.object == F_LIST && IsKeyPressed(KEY_ENTER)) {
        AppEvent selectEvent = {
            .type = EVENT_ITEM_SELECTED,
            .data.selection.selectedIndex = state->focus.index
        };
        EventQueuePush(eventQueue, selectEvent);
    }
    
    // Trigger search by enter
    if (state->focus.object == F_TEXTBOX && IsKeyPressed(KEY_ENTER)) {
        AppEvent searchEvent = {
            .type = EVENT_SEARCH_TRIGGERED,
        };
        EventQueuePush(eventQueue, searchEvent);
    }
}

int main(void) {    
    SetTraceLogLevel(LOG_DEBUG);

    WindowConfig config = {
        .refreshRate = 60,
        .windowPercentWidth = 0.8f,
        .windowPercentHeight = 0.7f,
        .opacity = 0.9f,
        .decoration = false
    };
    
    WindowDimensions windowDimensions = InitWindowConfig(&config, "lofi");

    AppState *state = (AppState *)malloc(sizeof(AppState));
    EventQueue *eventQueue = (EventQueue *)malloc(sizeof(EventQueue));
    ModeManager *modeManager = (ModeManager *)malloc(sizeof(ModeManager));
    
    if (!state || !eventQueue || !modeManager) {
        TraceLog(LOG_ERROR, "STATE: Allocation failed.");

        if (state) free(state);
        if (eventQueue) free(eventQueue);
        if (modeManager) free(modeManager);
        CloseWindow();
        return 1;
    }
    
    InitAppState(state);
    InitEventQueue(eventQueue);
    InitModeManager(modeManager);

    TraceLog(LOG_DEBUG, "STATE: Initialization complete.");
    
    modeManager->luaState = InitLua("init.lua", modeManager);
    if (!modeManager->luaState) {
        TraceLog(LOG_ERROR, "LUA: Failed to initialize!");
        CleanupAppState(state);
        free(state); free(eventQueue); free(modeManager);
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {
        // Process input and generate events
        ProcessInput(state, eventQueue);

        // Process all pending events
        while (!IsEventQueueEmpty(eventQueue)) {
            AppEvent event = EventQueuePop(eventQueue);
            HandleAppEvent(state, modeManager, event);
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawUI(state, windowDimensions);

        EndDrawing();
    }

    // Cleanup
    CleanupAppState(state);
    free(state); free(eventQueue); free(modeManager);
    CloseWindow();

    return 0;
}
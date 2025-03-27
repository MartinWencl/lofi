#include "ui.h"
#include "ui/draw.h"
#include <stdbool.h>
#include <string.h>

// WARNING: for testing only this unit shouldnt contain raylib
#include "raylib.h"

typedef enum {
    SPLIT_HORIZONTAL,  // Split along width
    SPLIT_VERTICAL     // Split along height
} SplitDirection;

typedef struct {
    Rectangle first;
    Rectangle second;
} SplitRectResult;

SplitRectResult SplitRectangle(Rectangle originalRect, float splitPercentage, SplitDirection direction) {
    SplitRectResult result;
    
    if (direction == SPLIT_VERTICAL) {
        // Split along width
        result.first = (Rectangle){
            .x = originalRect.x,
            .y = originalRect.y,
            .width = originalRect.width * splitPercentage,
            .height = originalRect.height
        };
        
        result.second = (Rectangle){
            .x = originalRect.x + (originalRect.width * splitPercentage),
            .y = originalRect.y,
            .width = originalRect.width * (1.0f - splitPercentage),
            .height = originalRect.height
        };
    } else { // SPLIT_VERTICAL
        // Split along height
        result.first = (Rectangle){
            .x = originalRect.x,
            .y = originalRect.y,
            .width = originalRect.width,
            .height = originalRect.height * splitPercentage
        };
        
        result.second = (Rectangle){
            .x = originalRect.x,
            .y = originalRect.y + (originalRect.height * splitPercentage),
            .width = originalRect.width,
            .height = originalRect.height * (1.0f - splitPercentage)
        };
    }
    
    return result;
}

void DrawUI(State* state, Dimensions windowDimensions) {
    Rectangle window = (Rectangle) {
        0,
        0,
        windowDimensions.width,
        windowDimensions.height
    };

    SplitRectResult splitRect = SplitRectangle(window, 0.25f,  SPLIT_HORIZONTAL);
    Rectangle textBoxRect = splitRect.first;
    Rectangle listViewRect = splitRect.second;

    bool textboxEditMode = state->focus.index == -1;
    
    DrawInputBox(&state->ui, textBoxRect, state->input, INPUT_TEXT_MAX_SIZE, textboxEditMode);
    DrawListView(&state->ui, listViewRect, (const char**)state->list, state->listCount, &state->focus.index);
}

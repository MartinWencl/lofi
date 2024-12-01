#include "ui.h"

void DrawUI(AppState* state, WindowDimensions windowDimensions) {
    // Calculate dimensions for the textbox
    Rectangle textBoxRect = (Rectangle){ 
        windowDimensions.width * 0.05f, // 5% from the left side of the window
        windowDimensions.height * 0.05f, // 5% from the top of the window
        windowDimensions.width * 0.9f, // 90% width of the window
        windowDimensions.height * 0.15f // 15% height of the window
    };

    // Calculate dimensions for the list view
    Rectangle listViewRect = (Rectangle){
        windowDimensions.width * 0.05f, // 5% from the left side of the window
        windowDimensions.height * 0.25f, // Below the textbox, with some space
        windowDimensions.width * 0.9f, // 90% width of the window
        windowDimensions.height * 0.7f // 70% height of the window, making up most of the remaining space
    };

    // Drawing the textbox and list view
    bool textBoxEditMode = (state->focus.object == F_TEXTBOX);
    DrawSimplifiedInputBox(textBoxRect, state->inputText, INPUT_TEXT_MAX_SIZE, textBoxEditMode);

    if (state->listCount > 0) {
        int activeIndex = (state->focus.index == -1) ? 0 : state->focus.index;

        // Update ListViewEx call to use new listViewRect
        ListViewEx(listViewRect, (const char**)state->listItems, state->listCount, &activeIndex, &state->listScrollIndex);
    }
}
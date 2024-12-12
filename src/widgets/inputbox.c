#include "inputbox.h"
#include <stdbool.h>

void DrawSimplifiedInputBox(Rectangle rect, char* text, int maxSize, bool editMode) {
    int fontSize = rect.height / 2; // Dynamically set the font size based on the box height
    
    // Draw the textbox background and outline
    DrawRectangleRec(rect, LIGHTGRAY); // Background
    DrawRectangleLinesEx(rect, 1, BLACK); // Outline

    // Measure the text size
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);

    // Calculate text position with padding
    Vector2 textPos = {
        rect.x + 4, 
        rect.y + (rect.height - textSize.y) / 2
    };
    
    // Draw text
    DrawTextEx(GetFontDefault(), text, textPos, fontSize, 1, BLACK);

    if (editMode) {
        // Get the width of the text to calculate the cursor's position
        int textWidth = MeasureTextEx(GetFontDefault(), text, fontSize, 1).x;
        int cursorPosX = rect.x + textWidth + 6; // Start a bit after the text ends

        // Ensure the cursor does not go outside the textbox
        if (cursorPosX > rect.x + rect.width - 4) {
            cursorPosX = rect.x + rect.width - 4;
        }

        // Draw cursor (a simple line for this example)
        DrawLine(cursorPosX, rect.y + 4, cursorPosX, rect.y + rect.height - 4, BLACK);
    }
}   
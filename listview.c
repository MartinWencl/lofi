#include "raylib.h"

#define MAX_LIST_VIEW_ITEMS 512

// Clamps a value `n` between `min` and `max`
int ClampInt(int n, int min, int max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

// Utility function to draw text with an optional background
// Ensures text is centered within a given rectangle.
static void DrawTextCentered(const char *text, Rectangle rec, Color textColor) {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 10, 1);
    Vector2 position = { rec.x + (rec.width - textSize.x) / 2, rec.y + (rec.height - textSize.y) / 2 };
    DrawText(text, position.x, position.y, 10, textColor);
}

void EnsureVisible(int itemCount, int visibleItems, int *scrollIndex, int active) {
    if (active < *scrollIndex) {
        *scrollIndex = active;
    }
    else if (active >= *scrollIndex + visibleItems) {
        int newScrollIndex = active - visibleItems + 1;
        *scrollIndex = ClampInt(newScrollIndex, 0, itemCount - visibleItems);
    }
}

// Simplified ListViewEx implementation using only raylib
int ListViewEx(Rectangle bounds, const char **textItems, int itemCount, int *selected, int *scrollIndex) {
    int visibleItems = bounds.height / 25; // Assuming each item is 25 pixels height
    int result = -1; // Result to return, -1 means no item is active
    
    // Ensure selected item is visible
    if(*selected != -1) {
        EnsureVisible(itemCount, visibleItems, scrollIndex, *selected);
    }

    // Calculate required scrollbar
    bool requireScroll = itemCount > visibleItems;
    if (requireScroll) {
        //bounds.width -= 12; // Make room for scrollbar
    }

    // Background
    DrawRectangleRec(bounds, LIGHTGRAY);

    // Items
    Rectangle itemBounds;
    itemBounds.x = bounds.x + 2;
    itemBounds.y = bounds.y + 2;
    itemBounds.width = bounds.width - 4;
    itemBounds.height = 20;

    for (int i = *scrollIndex; i < itemCount && i < *scrollIndex + visibleItems; i++) {
        itemBounds.y = bounds.y + 2 + (i - *scrollIndex) * 25;

        // Highlight the focused or selected item

        if (i == *selected) {
            DrawRectangleRec(itemBounds, GRAY);
        }

        DrawTextCentered(textItems[i], itemBounds, BLACK);
    }
    
    return result;
}
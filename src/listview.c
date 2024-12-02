#include "raylib.h"

#define MAX_LIST_VIEW_ITEMS 512

// Clamps a value `n` between `min` and `max`
int ClampInt(int n, int min, int max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
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

void DrawTextCenteredDynamic(const char *text, Rectangle rec, int fontSize, Color textColor) {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 position = {
        rec.x + (rec.width - textSize.x) / 2,
        rec.y + (rec.height - textSize.y) / 2
    };
    DrawTextEx(GetFontDefault(), text, position, fontSize, 1, textColor);
}

// Modify existing ListViewEx to scale item heights and text sizes
int ListViewEx(Rectangle bounds, const char **textItems, int itemCount, int *selected, int *scrollIndex) {
    // Calculate visible items based on a dynamic item height
    // Let's allocate 1/10th of the bounds height to each item, as an example
    int itemHeight = bounds.height / 5;
    int visibleItems = bounds.height / itemHeight;
    int result = -1;

    // Determine font size dynamically based on item height
    int fontSize = itemHeight / 2; // Half of the item height seems visually balanced

    // Ensure selected item is visible
    if (*selected != -1) {
        EnsureVisible(itemCount, visibleItems, scrollIndex, *selected);
    }

    // Items
    Rectangle itemBounds;
    itemBounds.x = bounds.x + 2;
    itemBounds.y = bounds.y + 2;
    itemBounds.width = bounds.width - 4;
    itemBounds.height = itemHeight - 4; // Deduct some to account for spacing

    for (int i = *scrollIndex; i < itemCount && i < *scrollIndex + visibleItems; i++) {
        itemBounds.y = bounds.y + 2 + (i - *scrollIndex) * itemHeight;
        // Highlight the focused or selected item
        if (i == *selected) {
            DrawRectangleRec(itemBounds, GRAY);
        }
        DrawTextCenteredDynamic(textItems[i], itemBounds, fontSize, BLACK);
    }

    return result;
}

#include "ui/draw.h"
#include "state.h"
#include <math.h>
// #include <stddef.h>
#include <string.h>

void DrawTextRec(Font font, const char* text, Rectangle bounds, float fontSize, float spacing, bool wordWrap, Color color) {
    if (text == NULL || strlen(text) == 0) return;
    
    float x = bounds.x;
    float y = bounds.y;
    float lineHeight = fontSize * 1.5f;  // Use 1.5x fontSize for line height
    
    // Get each word
    char word[256] = {0};
    int wordIndex = 0;
    int textLength = strlen(text);
    
    for (int i = 0; i <= textLength; i++) {
        // End of word or string
        if (text[i] == ' ' || text[i] == '\0') {
            word[wordIndex] = '\0';  // Null terminate the word
            
            // Measure word width
            Vector2 wordSize = MeasureTextEx(font, word, fontSize, spacing);
            
            // Check if word exceeds bounds width
            if (wordWrap && x + wordSize.x > bounds.x + bounds.width) {
                // Move to next line
                x = bounds.x;
                y += lineHeight;
            }
            
            // Draw word if within vertical bounds
            if (y + fontSize <= bounds.y + bounds.height) {
                DrawTextEx(font, word, (Vector2){x, y}, fontSize, spacing, color);
            }
            
            // Update x position for next word
            x += wordSize.x + MeasureTextEx(font, " ", fontSize, spacing).x;
            
            // Reset word buffer
            wordIndex = 0;
        }
        else {
            // Build word
            word[wordIndex++] = text[i];
        }
    }
}

void DrawInputBox(UIState* state, Rectangle bounds, char* text, int maxSize, bool captureInput) {
    // Draw background
    Color bgColor = captureInput ? state->loaded.theme.selected : state->loaded.theme.background;
    DrawRectangleRec(bounds, bgColor);
    
    // Draw border/outline
    Color borderColor = captureInput ? state->loaded.theme.accent : state->loaded.theme.border;
    DrawRectangleLinesEx(bounds, 1.0f, borderColor);
    
    // Calculate text bounds with padding
    Rectangle textBounds = {
        bounds.x + PADDING,
        bounds.y + (bounds.height - state->config.font.size) / 2,
        bounds.width - 2 * PADDING,
        state->config.font.size
    };
    
    // Draw text
    Font font = state->loaded.font;
    DrawTextEx(font, text, (Vector2){textBounds.x, textBounds.y}, 
              state->config.font.size, 2.0f, state->loaded.theme.text);
}

void DrawListView(UIState* state, Rectangle bounds, const char** items, int count, int* selectedIndex) {
    // Draw background
    DrawRectangleRec(bounds, state->loaded.theme.background);
    if(*selectedIndex < 0) {
        return;
    }

    // Calculate item height to fit approximately 5 items, but ensure it fits within bounds
    int visibleItems = 5;
    float itemHeight = bounds.height / visibleItems;
    // Ensure item height is not smaller than the font size
    if (itemHeight < state->config.font.size) {
        itemHeight = state->config.font.size;
        visibleItems = (int)(bounds.height / itemHeight);
    }

    // Calculate scroll position based on focused item
    int scrollIndex = *selectedIndex - (visibleItems / 2);
    
    // Clamp scroll index to valid range
    if (scrollIndex < 0) {
        scrollIndex = 0;
    }
    int maxScroll = count - visibleItems;
    if (maxScroll < 0) maxScroll = 0;
    if (scrollIndex > maxScroll) {
        scrollIndex = maxScroll;
    }

    int endIndex = scrollIndex + visibleItems;
    if (endIndex > count) endIndex = count;

    // Draw items
    for (int i = scrollIndex; i < endIndex; i++) {
        Rectangle itemBounds = {
            bounds.x,
            bounds.y + (i - scrollIndex) * itemHeight,
            bounds.width,
            itemHeight
        };
        // Draw selection highlight
        if (i == *selectedIndex) {
            DrawRectangleRec(itemBounds, state->loaded.theme.selected);
        }
        // Draw item text
        Vector2 textPos = {
            itemBounds.x + PADDING,
            itemBounds.y + (itemHeight - state->config.font.size) / 2
        };
        Font font = state->loaded.font;
        DrawTextEx(font, items[i], textPos, state->config.font.size, 2.0f, state->loaded.theme.text);
    }

    // Draw scrollbar if needed
    if (count > visibleItems) {
        float scrollbarWidth = 8;
        Rectangle scrollbarBounds = {
            bounds.x + bounds.width - scrollbarWidth,
            bounds.y,
            scrollbarWidth,
            bounds.height
        };
        float scrollPercent = maxScroll > 0 ? (float)scrollIndex / (float)maxScroll : 0;
        float scrollHandleHeight = (float)visibleItems / (float)count * bounds.height;
        float scrollHandleY = bounds.y + scrollPercent * (bounds.height - scrollHandleHeight);
        DrawRectangleRec(scrollbarBounds, (Color){ 64, 64, 64, 255 });
        DrawRectangle(
            scrollbarBounds.x,
            scrollHandleY,
            scrollbarWidth,
            scrollHandleHeight,
            (Color){ 128, 128, 128, 255 }
        );
    }
}

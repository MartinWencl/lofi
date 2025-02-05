#include "ui/draw.h"
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

void DrawInputBox(Rectangle bounds, char* text, int maxSize, bool captureInput) {
    // Draw background
    Color bgColor = captureInput ? ACTIVE_COLOR : INACTIVE_COLOR;
    DrawRectangleRec(bounds, bgColor);
    
    // Calculate text bounds with padding
    Rectangle textBounds = {
        bounds.x + PADDING,
        bounds.y + (bounds.height - TEXT_SIZE) / 2,
        bounds.width - 2 * PADDING,
        TEXT_SIZE
    };
    
    // Draw text
    DrawTextRec(GetFontDefault(), text, textBounds, TEXT_SIZE, 2.0f, true, TEXT_COLOR);
    
    // Draw cursor when in edit mode
    if (captureInput) {
        float time = GetTime();
        if (fmod(time, CURSOR_BLINK_RATE * 2) < CURSOR_BLINK_RATE) {
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, TEXT_SIZE, 2.0f);
            float cursorX = textBounds.x + textSize.x;
            if (cursorX < textBounds.x + textBounds.width) {
                DrawLineV(
                    (Vector2){ cursorX, textBounds.y },
                    (Vector2){ cursorX, textBounds.y + TEXT_SIZE },
                    TEXT_COLOR
                );
            }
        }
    }
}

void DrawListView(Rectangle bounds, const char** items, int count, int* selectedIndex, int scrollIndex) {
    // Draw background
    DrawRectangleRec(bounds, INACTIVE_COLOR);
    
    // Calculate visible items
    int visibleItems = (int)(bounds.height / ITEM_HEIGHT);
    int endIndex = scrollIndex + visibleItems;
    if (endIndex > count) endIndex = count;
    
    // Draw items
    for (int i = scrollIndex; i < endIndex; i++) {
        Rectangle itemBounds = {
            bounds.x,
            bounds.y + (i - scrollIndex) * ITEM_HEIGHT,
            bounds.width,
            ITEM_HEIGHT
        };
        
        // Draw selection highlight
        if (i == *selectedIndex) {
            DrawRectangleRec(itemBounds, ACTIVE_COLOR);
        }
        
        // Draw item text
        Vector2 textPos = {
            itemBounds.x + PADDING,
            itemBounds.y + (ITEM_HEIGHT - TEXT_SIZE) / 2
        };
        DrawTextEx(GetFontDefault(), items[i], textPos, TEXT_SIZE, 2.0f, TEXT_COLOR);
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
        
        float scrollPercent = (float)scrollIndex / (float)(count - visibleItems);
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

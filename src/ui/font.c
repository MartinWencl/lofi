#include "raylib.h"
#include "ui/state.h" // filling in the forward declaration in header
#include <fontconfig/fontconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LoadFontFromState(UIState* ui) {
    // Initialize FontConfig
    if (!FcInit()) {
        TraceLog(LOG_ERROR, "Failed to init FontConfig");
        return;
    }

    FcPattern* pattern = FcNameParse((const FcChar8*)ui->config.font.name);
    FcConfigSubstitute(NULL, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    char* fontPath = NULL;

    // Match the font
    FcResult result;
    FcPattern* font = FcFontMatch(NULL, pattern, &result);
    if (font) {
        FcChar8* path = NULL;
        if (FcPatternGetString(font, FC_FILE, 0, &path) == FcResultMatch) {
            fontPath = (char*)malloc(strlen((const char*)path) + 1);
            strcpy(fontPath, (const char*)path);
        }
        FcPatternDestroy(font);
    }
    FcPatternDestroy(pattern);

    if (fontPath) {
        ui->loaded.font = LoadFont(fontPath);
        free(fontPath);

        if (!IsFontValid(ui->loaded.font)) {
            TraceLog(LOG_WARNING, "Font loaded is not valid, using default font.");
        } else {
            TraceLog(LOG_INFO, "Font successfully loaded");
        }
    } else {
        TraceLog(LOG_WARNING, "Font not found, using default font.");
    }

    // Clean up FontConfig
    FcFini();
}

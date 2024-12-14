// tests/ui/test_window.c
#include <assert.h>
#include "ui/window.h"

void test_default_window_config() {
    assert(DEFAULT_WINDOW_CONFIG.windowPercentWidth == 0.3f);
    assert(DEFAULT_WINDOW_CONFIG.windowPercentHeight == 0.2f);
    assert(DEFAULT_WINDOW_CONFIG.opacity == 0.9f);
    assert(DEFAULT_WINDOW_CONFIG.decoration == false);
}

void test_get_safe_monitor_dimensions() {
    Dimensions dimensions = GetSafeMonitorDimensions();
    assert(dimensions.width > 0);
    assert(dimensions.height > 0);
}

void test_set_window_from_config() {
    WindowConfig config = DEFAULT_WINDOW_CONFIG;
    config.windowPercentWidth = 0.5f;
    config.windowPercentHeight = 0.5f;
    Dimensions dimensions = SetWindowFromConfig(&config);
    assert(dimensions.width > 0);
    assert(dimensions.height > 0);
}

int main() {
    test_default_window_config();
    test_get_safe_monitor_dimensions();
    test_set_window_from_config();
    return 0;
}

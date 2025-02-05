# tests/ui/CMakeLists.txt
file(GLOB_RECURSE TEST_UI_SOURCES *.c)

# Create the UI test executable
add_executable(test_ui ${TEST_UI_SOURCES})

# Include directories for tests
target_include_directories(test_ui PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${luajit_SOURCE_DIR}/src
)

# Link test executable to required libraries
target_link_libraries(test_ui 
    raylib
    ${luajit_SOURCE_DIR}/src/${LUA_LIB_FILE}
)

# Register the UI test
add_test(NAME test_ui COMMAND test_ui)

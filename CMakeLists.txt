cmake_minimum_required(VERSION 3.10)
project(RaylibProject C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set path variables
set(RAYLIB_PATH "C:/raylib/raylib" CACHE PATH "Path to Raylib")
set(COMPILER_PATH "C:/raylib/w64devkit/bin" CACHE PATH "Path to compiler")

# Source files
set(SOURCE_FILES
    main.c
    core.c
    events.c
    listview.c
    utils.c
    lua_core.c
)

# Include directories
include_directories(
    ${RAYLIB_PATH}/src
    ../
    ../lua
)

# Specify raylib static library location
set(RAYLIB_LIBRARY "${RAYLIB_PATH}/src/raylib.dll.rc")

# Add executable
add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES}
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -DPLATFORM_DESKTOP
)

# Find required packages
find_package(OpenGL REQUIRED)

# Define the locations of the DLL files
set(LUA_DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(LUAJIT_DLL "${LUA_DLL_PATH}/luajit.exe")
set(LUA51_DLL "${LUA_DLL_PATH}/lua51.dll")

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    "${RAYLIB_LIBRARY}"  # Link directly to raylib.a
    opengl32
    gdi32
    winmm
    "${LUAJIT_DLL}"
    "${LUA51_DLL}"
)

# Set static linking for other libraries
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS_RELEASE "-static"
)

# Copy DLL files to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LUAJIT_DLL}"
        "${LUA51_DLL}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
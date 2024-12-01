cmake_minimum_required(VERSION 3.12)
project(lofi C)

# Set C Standard
set(CMAKE_C_STANDARD 11)

# Fetch and Build LuaJIT with MinGW
include(FetchContent)
FetchContent_Declare(
    luajit
    GIT_REPOSITORY https://github.com/LuaJIT/LuaJIT.git
    GIT_TAG master
)
FetchContent_MakeAvailable(luajit)

# Build command for LuaJIT
if(WIN32)
    execute_process(
        COMMAND mingw32-make
        WORKING_DIRECTORY ${luajit_SOURCE_DIR}/src
    )
endif()

# Fetch and Build Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

# Add executable
add_executable(lofi 
    src/main.c
    src/core.c
    src/events.c
    src/listview.c
    src/lua_core.c
    src/utils.c
    src/modes.c
    src/startup.c
    src/ui.c
    src/inputbox.c
)

# Include directories
target_include_directories(lofi PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${luajit_SOURCE_DIR}/src
)

# Ensure the built library is correctly referenced
set(LUA_LIB_FILE "lua51.dll") # for static linking, use the .a file

# Link libraries
target_link_libraries(lofi 
    raylib
    ${luajit_SOURCE_DIR}/src/${LUA_LIB_FILE}
)

# Custom commands to install LuaJIT files
set(LUAJIT_INSTALL_DIR "${CMAKE_BINARY_DIR}")
add_custom_command(TARGET lofi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${luajit_SOURCE_DIR}/src/luajit.exe"
        "${LUAJIT_INSTALL_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${luajit_SOURCE_DIR}/src/lua51.dll"
        "${LUAJIT_INSTALL_DIR}"
    COMMENT "Copying LuaJIT binaries to the project build directory"
)
add_custom_command(TARGET lofi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LUAJIT_INSTALL_DIR}/lua/jit"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${luajit_SOURCE_DIR}/src/jit"
        "${LUAJIT_INSTALL_DIR}/lua/jit"
    COMMENT "Copying LuaJIT Lua files to the project build directory"
)
# Copy init.lua from the src directory to the build directory post-build
add_custom_command(TARGET lofi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/src/init.lua"
        "${CMAKE_BINARY_DIR}/init.lua"
    COMMENT "Copying init.lua to the project build directory"
)

if(WIN32)
    target_link_libraries(lofi 
        -lwinmm -static
    )
elseif(UNIX)
    target_link_libraries(lofi 
        dl
        m
    )
endif()

if(MSVC)
    set_target_properties(lofi PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()
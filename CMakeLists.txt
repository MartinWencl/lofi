cmake_minimum_required(VERSION 3.12)
project(lofi C)

# Set C Standard
set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

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
elseif(UNIX)
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${luajit_SOURCE_DIR}
    )
endif()

# Fetch and Build Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

# Find all .c source files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCE_FILES src/*.c)

# Create the executable with all found .c files
add_executable(lofi ${SOURCE_FILES})

# Include directories
target_include_directories(lofi PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${luajit_SOURCE_DIR}/src
)

# Ensure the built library is correctly referenced
    if(WIN32)
        set(LUA_LIB_FILE "lua51.dll") # for static linking, use the .a file
    elseif(UNIX)
        set(LUA_LIB_FILE "libluajit.so") # for static linking, use the .a file
    endif()
    
# Link libraries
target_link_libraries(lofi 
    raylib
    ${luajit_SOURCE_DIR}/src/${LUA_LIB_FILE}
)

# Custom commands to install LuaJIT files
set(LUAJIT_INSTALL_DIR "${CMAKE_BINARY_DIR}")
if(WIN32)
    add_custom_command(TARGET lofi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${luajit_SOURCE_DIR}/src/luajit.exe"
            "${LUAJIT_INSTALL_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${luajit_SOURCE_DIR}/src/${LUA_LIB_FILE}"
            "${LUAJIT_INSTALL_DIR}"
        COMMENT "Copying LuaJIT binaries to the project build directory"
    )
elseif(UNIX)
    add_custom_command(TARGET lofi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${luajit_SOURCE_DIR}/src/luajit"
            "${LUAJIT_INSTALL_DIR}/luajit"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${luajit_SOURCE_DIR}/src/${LUA_LIB_FILE}"
            "${LUAJIT_INSTALL_DIR}/libluajit-5.1.so.2"
        COMMENT "Copying LuaJIT binaries to the project build directory"
    )
endif()

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

# Find FontConfig package and include it
find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

# Link FontConfig libraries
target_include_directories(lofi PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
target_link_libraries(lofi ${FONTCONFIG_LIBRARIES})
target_compile_options(lofi PRIVATE ${FONTCONFIG_CFLAGS_OTHER})

enable_testing()

add_subdirectory(tests/ui)

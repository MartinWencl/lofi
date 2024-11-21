cmake_minimum_required(VERSION 3.15)
project(MyProject C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(APP_NAME "lofi")

# Build options
option(BUILD_STATIC "Build static executable" OFF)

include(ExternalProject)
include(FetchContent)

# Detect toolchain
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(USING_GCC 1)
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(USING_MSVC 1)
endif()

if(WIN32 AND USING_GCC)
    set(USING_MINGW 1)
endif()

# Platform-specific settings
if(USING_MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++")
endif()

# LuaJIT setup
if(USING_MSVC)
    set(LUAJIT_MAKE_CMD msvcbuild.bat)
    set(LUAJIT_BUILD_OPTIONS "static") # Add any desired options here, e.g., "debug static"
    set(LUAJIT_FILE_NAME lua51.lib) # Use lua51.lib instead of lua51.dll
elseif(USING_MINGW)
    set(LUAJIT_MAKE_CMD mingw32-make)
    set(LUAJIT_MAKE_FLAGS HOST_CC="gcc -m32" CROSS=x86_64-w64-mingw32- TARGET_SYS=Windows)
    set(LUAJIT_FILE_NAME libluajit.a)
else()
    set(LUAJIT_MAKE_CMD make)
    set(LUAJIT_MAKE_FLAGS TARGET_CFLAGS=-fPIC)
    set(LUAJIT_FILE_NAME libluajit.a)
endif()

set(LUAJIT_STATIC_LIB ${CMAKE_BINARY_DIR}/deps/luajit/src/luajit_external/src/${LUAJIT_FILE_NAME})

ExternalProject_Add(
    luajit_external
    GIT_REPOSITORY https://github.com/LuaJIT/LuaJIT.git
    GIT_TAG v2.1
    PREFIX ${CMAKE_BINARY_DIR}/deps/luajit
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND
        ${CMAKE_COMMAND} -E chdir src ${LUAJIT_MAKE_CMD} ${LUAJIT_BUILD_OPTIONS}
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${LUAJIT_STATIC_LIB}
)

# raylib setup
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)

# Configure raylib build options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# Include and link LuaJIT
include_directories(${CMAKE_BINARY_DIR}/deps/luajit/src/luajit_external/src/)
add_library(luajit_static STATIC IMPORTED GLOBAL)
add_dependencies(luajit_static luajit_external)
set_target_properties(luajit_static PROPERTIES
    IMPORTED_LOCATION ${LUAJIT_STATIC_LIB}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/deps/luajit/src/luajit_external/src/
)

# Include raylib
include_directories(${raylib_SOURCE_DIR}/src)
# Include LuaJIT headers 
include_directories(${CMAKE_BINARY_DIR}/deps/luajit/src/luajit/src) 
# Gather all .c files in src directory 
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c") 
# Your executable 
add_executable(lofi ${SRC_FILES})

if(BUILD_STATIC)
    if(UNIX)
        target_link_options(${APP_NAME} PRIVATE -static-libgcc -static)
        target_link_libraries(${APP_NAME} PRIVATE dl m)
    elseif(USING_MINGW)
        target_link_libraries(${APP_NAME} PRIVATE
            mingw32
            pthread
            winmm
            gdi32
            kernel32
        )
    endif()
endif()

target_link_libraries(${APP_NAME} PRIVATE
    raylib
    luajit_static
)

if(USING_MSVC)
    target_compile_options(${APP_NAME} PRIVATE /W4)
else()
    target_compile_options(${APP_NAME} PRIVATE -Wall -Wextra)
endif()

target_compile_definitions(${APP_NAME} PRIVATE ${PLATFORM_DEFINES})

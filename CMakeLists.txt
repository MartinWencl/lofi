cmake_minimum_required(VERSION 3.12)
project(lofi C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_C_COMPILER clang)

file(GLOB_RECURSE SOURCE_FILES src/*.c)

add_executable(lofi ${SOURCE_FILES})

target_include_directories(lofi PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/callbacks
    ${CMAKE_SOURCE_DIR}/src/ui
)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
pkg_check_modules(LUAJIT REQUIRED luajit)
pkg_check_modules(RAYLIB REQUIRED raylib)
pkg_check_modules(GLFW REQUIRED glfw3)

# Include directories and link libraries for FontConfig
target_include_directories(lofi PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
target_link_libraries(lofi ${FONTCONFIG_LIBRARIES})
target_compile_options(lofi PRIVATE ${FONTCONFIG_CFLAGS_OTHER})

# Include directories and link libraries for LuaJIT
target_include_directories(lofi PRIVATE ${LUAJIT_INCLUDE_DIRS})
target_link_libraries(lofi ${LUAJIT_LIBRARIES})
target_compile_options(lofi PRIVATE ${LUAJIT_CFLAGS_OTHER})

# Include directories and link libraries for Raylib
target_include_directories(lofi PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(lofi ${RAYLIB_LIBRARIES})
target_compile_options(lofi PRIVATE ${RAYLIB_CFLAGS_OTHER})

target_include_directories(lofi PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(lofi ${GLFW_LIBRARIES})
target_compile_options(lofi PRIVATE ${GLFW_CFLAGS_OTHER})

# Copy init.lua from the src directory to the build directory post-build
add_custom_command(TARGET lofi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/src/init.lua"
        "${CMAKE_BINARY_DIR}/init.lua"
    COMMENT "Copying init.lua to the project build directory"
)

# Link the libraries to the executable
target_link_libraries(lofi ${FONTCONFIG_LIBRARIES} ${LUAJIT_LIBRARIES} ${RAYLIB_LIBRARIES} -lm)

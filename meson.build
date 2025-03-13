project('lofi', 'c',
  version : '0.1',
  default_options : [
    'c_std=c23',
    'warning_level=2',
    'buildtype=release',
    'c_compiler=clang'
  ]
)

cmake = import('cmake')

# Dependencies
fontconfig_dep = dependency('fontconfig')
luajit_dep = dependency('luajit')
glfw_dep = dependency('glfw3')
opengl_dep = dependency('gl')
xorg = dependency('x11')

# ld_library_path = run_command('echo', '$LD_LIBRARY_PATH').stdout().strip()

# Configure and build raylib using CMake
# raylib_proj = cmake.subproject('raylib',
#   cmake_options: [
#     '-DBUILD_EXAMPLES=OFF',
#     # '-DUSE_EXTERNAL_GLFW=ON',
#     # '-DPLATFORM_SUPPORTS_WAYLAND=ON',
#     '-DOPENGL_INCLUDE_DIR=' + opengl_dep.get_variable(pkgconfig: 'includedir'),
#     '-DOPENGL_opengl_LIBRARY=' + opengl_dep.get_variable(pkgconfig: 'libdir'),
#     # '-DGLFW_BUILD_WAYLAND=ON'
#     '-DX11_X11_INCLUDE_PATH=/nix/store/3gbmgijyy53d1cm551zxhgaglkvvv44m-libX11-1.8.11/include',
#     '-DX11_X11_LIB=/nix/store/3gbmgijyy53d1cm551zxhgaglkvvv44m-libX11-1.8.11/lib',
#     '-DLD_LIBRARY_PATH=' + ld_library_path,
#     # 
#   ]
# )
# raylib_dep = raylib_proj.dependency('raylib')
raylib_dep = dependency('raylib')

# Source files
src_files = []
foreach d : ['src', 'src/callbacks', 'src/ui']
  src_files += files(
    run_command('find', 
      meson.current_source_dir() / d, 
      '-name', '*.c',
      check: true
    ).stdout().strip().split('\n')
  )
endforeach

# Include directories
inc_dirs = include_directories(
  'src',
  'src/callbacks',
  'src/ui'
)

# Main executable
exe = executable('lofi',
  src_files,
  include_directories : inc_dirs,
  dependencies : [
    fontconfig_dep,
    luajit_dep,
    glfw_dep,
    raylib_dep,
    opengl_dep,
    xorg,
  ],
  link_args : [ '-lm' ],
  install : true
)

# Copy init.lua to build directory
configure_file(
  input: 'src/init.lua',
  output: 'init.lua',
  copy: true
)

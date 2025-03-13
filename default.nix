let
  pkgs = import <nixpkgs> {
    config = { };
  };

in
pkgs.mkShell {
  # NIX_DEBUG_INFO_DIRS = "${pkgs.lib.getLib myDebugInfoDirs}/lib/debug";

  nativeBuildInputs = with pkgs; [ clang bat gdb valgrind pkg-config meson ninja ];
  buildInputs = [
    pkgs.xorg.libX11
    pkgs.xorg.xorgproto
    pkgs.xorg.libXext
    pkgs.xorg.libXrandr
    pkgs.xorg.libXinerama
    pkgs.xorg.libXcursor
    pkgs.xorg.libXi
    pkgs.wlroots
    pkgs.libGL
    pkgs.dlib
    pkgs.mlib
    pkgs.glibc
    pkgs.fontconfig
    pkgs.expat
    pkgs.luajit
    pkgs.raylib
    # pkgs.glfw-wayland
    pkgs.glfw
    pkgs.uthash
  ];

  shellHook = ''
    export LD_LIBRARY_PATH="$PWD/build:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.raylib}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.glibc}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.fontconfig}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.glfw}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.uthash}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.xorg.libX11}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.xorg.libXrandr}/lib:$LD_LIBRARY_PATH"
    export X11_X11_LIB="${pkgs.xorg.libX11}/lib"
    export X11_X11_INCLUDE_PATH="${pkgs.xorg.libX11}/include"
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${pkgs.raylib}/lib:${pkgs.glfw}/lib:${pkgs.fontconfig}/lib:${pkgs.expat}/lib:${pkgs.luajit}/lib:${pkgs.raylib}/lib:${pkgs.glfw-wayland}/lib:${pkgs.uthash}/include"
    export PATH="$PATH:${pkgs.clang}"
    export CMAKE_PREFIX_PATH="CMAKE_PREFIX_PATH:${pkgs.glfw}/lib:/nix/store/kixp27k45r663ajjd154di0cgbfw132l-glfw-3.4/lib/cmake/glfw3"
  '';
}

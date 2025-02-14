let
  pkgs = import <nixpkgs> {
    config = { };
  };

in
pkgs.mkShell {
  # NIX_DEBUG_INFO_DIRS = "${pkgs.lib.getLib myDebugInfoDirs}/lib/debug";

  nativeBuildInputs = with pkgs; [ clang gdb valgrind pkg-config ];
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
    pkgs.glfw
    pkgs.uthash
  ];

  shellHook = ''
    export LD_LIBRARY_PATH="$PWD/build:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.glibc}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.fontconfig}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.glfw}/lib:$LD_LIBRARY_PATH"
    export LD_LIBRARY_PATH="${pkgs.uthash}/lib:$LD_LIBRARY_PATH"
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${pkgs.glfw}/lib:${pkgs.fontconfig}/lib:${pkgs.expat}/lib:${pkgs.luajit}/lib:${pkgs.raylib}/lib:${pkgs.glfw3}/lib:${pkgs.uthash}/include""
  '';
}

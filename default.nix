let
  pkgs = import <nixpkgs> {
    config = { };
  };

  # myDebugInfoDirs = pkgs.symlinkJoin {
  #   name = "myDebugInfoDirs";
  #   paths = with pkgs; [
  #     glibc.debug
  #   ];
  # };


in
pkgs.mkShell {
  # NIX_DEBUG_INFO_DIRS = "${pkgs.lib.getLib myDebugInfoDirs}/lib/debug";

  nativeBuildInputs = with pkgs; [ cmake clang gdb valgrind pkg-config ];
  buildInputs = [
    pkgs.xorg.libX11
    pkgs.xorg.xorgproto
    pkgs.xorg.libXext
    pkgs.xorg.libXrandr
    pkgs.xorg.libXinerama
    pkgs.xorg.libXcursor
    pkgs.xorg.libXi
    pkgs.libGL
    pkgs.dlib
    pkgs.mlib
    pkgs.glibc
    pkgs.fontconfig
    pkgs.expat
  ];

  shellHook = ''
    export LD_LIBRARY_PATH="$PWD/build:$LD_LIBRARY_PATH"
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${pkgs.fontconfig}/lib:${pkgs.expat}/lib"
  '';
}

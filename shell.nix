let
  nixpkgs-src = builtins.fetchGit {
    # 23.05
    url = "https://github.com/NixOS/nixpkgs.git";
  };

  pkgs = import nixpkgs-src {
    config = {
      # allowUnfree may be necessary for some packages, but in general you should not need it.
      allowUnfree = false;
    };
  };

  libraries = [
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
  ];

  lib-path = with pkgs; lib.makeLibraryPath libraries;

  shell = pkgs.mkShell {
    buildInputs = [
      pkgs.cmake
    ] ++ libraries;

    shellHook = ''
      export "LD_LIBRARY_PATH=${lib-path}"
      export "LIBCLANG_PATH=${pkgs.libclang.lib}/lib";
    '';
  };

in shell

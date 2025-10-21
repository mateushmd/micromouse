{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    libsForQt5.qtbase
    libsForQt5.qt5.qtxmlpatterns
    libsForQt5.libqglviewer
    libsForQt5.qt5.qtwayland
    libGL
  ];
}

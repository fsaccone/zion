let
  pkgs = import <nixpkgs> { };

  crossPkgs = import <nixpkgs> {
    crossSystem.config = "riscv64-none-elf";
  };
in
crossPkgs.mkShell {
  nativeBuildInputs = [
    pkgs.pdpmake
    pkgs.qemu
  ];
}

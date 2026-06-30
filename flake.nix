# Credits:
# https://parkerjones.dev/posts/mastering-c-nix-flake/

{
  description = "OS dev";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: {
    packages = nixpkgs.lib.genAttrs [ "x86_64-linux" ] (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in
    rec {
      myos = pkgs.stdenv.mkDerivation {
        pname = "myos";
        version = "0.0.1";

        src = ./src;

        nativeBuildInputs = with pkgs; [ 
          pkgsCross.i686-embedded.buildPackages.gcc
          grub2
          libisoburn
        ];

        buildInputs = with pkgs; [ 
          qemu
        ];

        phases = [ "preBuildPhase" "buildPhase" "preInstallPhase" "installPhase" ];

        preBuildPhase = ''
          cd $src
          pwd
        '';

        buildPhase = '' 
          i686-elf-as boot.s -o $TMPDIR/boot.o
          i686-elf-gcc -c kernel.c -o $TMPDIR/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
          i686-elf-gcc -T linker.ld -o $TMPDIR/myos -ffreestanding -O2 -nostdlib $TMPDIR/boot.o $TMPDIR/kernel.o -lgcc
        '';

        preInstallPhase = ''
          mkdir -p $out/bin
          mkdir -p $TMPDIR/isodir/boot/grub/
          cp grub.cfg $TMPDIR/isodir/boot/grub/grub.cfg
          cp $TMPDIR/myos $TMPDIR/isodir/boot/myos
        '';

        installPhase = ''
          grub-mkrescue -o $TMPDIR/myos.iso $TMPDIR/isodir
          mv $TMPDIR/myos.iso $TMPDIR/myos $out/bin/
        '';

        meta = with pkgs.lib; {
          description = "An OS dev test";
          license = licenses.mit;
          maintainers = [ maintainers.c6rg0 ];
          platforms = platforms.unix;
        };
      };
    });

    defaultPackage = {
      x86_64-linux = self.packages.x86_64-linux.myos;
    };
  };
}

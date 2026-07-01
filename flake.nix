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

        src = ./.;

        nativeBuildInputs = with pkgs; [ 
          pkgsCross.i686-embedded.buildPackages.gcc
          grub2
          libisoburn
          gnumake
        ];

        buildInputs = with pkgs; [ 
          qemu
        ];

        # What not to run manually:
          # $ runHoot ...
          # $ make install ...
          # anything with $out/ or $TMPDIR

        buildPhase = '' 
          runHook preBuild
          make
          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall
          mkdir -p $out/bin
          make install PREFIX=$out/bin

          mkdir -p $TMPDIR/isodir/boot/grub/
          cp src/grub.cfg $TMPDIR/isodir/boot/grub/grub.cfg
          cp $out/bin/kernel.elf $TMPDIR/isodir/boot/kernel.elf

          grub-mkrescue -o $out/bin/kernel.iso $TMPDIR/isodir
          runHook postInstall
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

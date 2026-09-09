# Credits:
# https://parkerjones.dev/posts/mastering-c-nix-flake/

{
  description = "OS dev";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: {
    packages = nixpkgs.lib.genAttrs [ "x86_64-linux" ] (
      system:
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
            clang-tools
          ];

          patchPhase = ''
            sed -i 's+export SYSROOT="$(pwd)/sysroot"+export SYSROOT="$TMPDIR/sysroot"+g' scripts/config.sh 
          '';

          configurePhase = ''
            set -e
            . scripts/config.sh
          '';

          buildPhase = ''
            runHook preBuild

            for PROJECT in $SYSTEM_HEADER_PROJECTS; do
              (cd $PROJECT && DESTDIR="$TMPDIR/sysroot" $MAKE install-headers)
            done

            for PROJECT in $PROJECTS; do
              (cd $PROJECT && DESTDIR="$TMPDIR/sysroot" $MAKE install)
            done

            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall

            mkdir -p $out/bin
            mkdir -p $TMPDIR/isodir/boot/grub/

            cp $TMPDIR/sysroot/boot/*.elf $TMPDIR/isodir/boot/myos.elf
            cp $TMPDIR/sysroot/boot/*.elf $out/bin/myos.elf
            cat > $TMPDIR/isodir/boot/grub/grub.cfg << EOF
            menuentry "myos" {
              multiboot /boot/myos.elf
            }
            EOF
            grub-mkrescue -o $out/bin/myos.iso $TMPDIR/isodir

          '';

          meta = with pkgs.lib; {
            description = "An OS dev test";
            license = licenses.mit;
            maintainers = [ maintainers.c6rg0 ];
            platforms = platforms.unix;
          };
        };
      }
    );

    defaultPackage = {
      x86_64-linux = self.packages.x86_64-linux.myos;
    };
  };
}

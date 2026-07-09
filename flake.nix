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

        patchPhase = ''
          sed -i 's+export SYSROOT="$(pwd)/sysroot"+export SYSROOT="$TMPDIR/sysroot"+g' config.sh 
        '';

        configurePhase = ''
          set -e
          . ./config.sh
        '';

        buildPhase = '' 
          runHook preBuild

          # headers.sh
          # mkdir -p "$SYSROOT"
          
          for PROJECT in $SYSTEM_HEADER_PROJECTS; do
            (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
          done

          # build.sh
          for PROJECT in $PROJECTS; do
            (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
          done

          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall

          mkdir -p $out/bin
          mkdir -p $out/share
          pwd >> $out/share/dir.txt
          ls >> $out/share/dir.txt

          mkdir -p $TMPDIR/isodir/boot/grub/

          cp $SYSROOT/boot/myos.elf $TMPDIR/isodir/boot/myos.elf
          cp $SYSROOT/boot/myos.elf $out/bin/myos.elf
          cat > $TMPDIR/isodir/boot/grub/grub.cfg << EOF
          menuentry "myos" {
            multiboot /boot/myos.elf
          }
          EOF
          grub-mkrescue -o $out/bin/myos.iso $TMPDIR/isodir

          #runHook postInstall
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

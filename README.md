# Bare-metal learning repo
- Use a UNIX(-like) OS to build (Linux, Macos, BSD, ...)
- Package manager personally used: nix
- Dependencies in flake.nix can be installed manually or with `$ nix develop`

# Build instructions
- Use build.sh to build (I'll set up "make" later),
- To use iso: `$ qemu-system-i386 -cdrom target/myos.iso`.


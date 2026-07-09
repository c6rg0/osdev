# Freestanding/bare-metal console

![showcase](showcase.png)

### Includes:
  - Multiboot header (for GRUB) and kernel entry
  - A kernel
  - Freestanding libc (libk.a)
  - VGA/tty driver
  - outb and inb (the most basic I/O)

### To do:
  - Add interupt support
  - Add a ps/2 (keyboard) driver
  - Make a (command line) shell
  - Set up a global descriptor table (GDT)
  - Get to ring 2 and 3; seperate kernel and user space

### Supported architectures:
  - i636
  - (will add more for fun later, the ones in mind are x86-64 and ARM64)

## NOTES:
- $ = shell command (assuming the use of bash && gnu coreutils).
- For windows users: use WSL with nixos and follow the nix instructions.
- You might have to compile the cross-compiler (which can take some time).

- Credits for initial tutorial: [wiki.osdev.org](https://wiki.osdev.org/Bare_Bones),
- and boilerplate code/structure: [wiki.osdev.org](https://wiki.osdev.org/Meaty_Skeleton).
- The MIT license for the tutorial is in the root of the directory.

## How to build with nix pacakge manager
1. Have nix(os) installed (supports Linux and Macos),
2. Download the repo:
  - `$ git clone https://github.com/c6rg0/Osdev.git`
  - `$ cd Osdev`
2. `$ nix build`

### Manually
1. Use the `flake.nix` to read the dependencies:
  - `nativeBuildInputs`: compile-time dependencies. `buildInputs`: run-time dependencies.
2. (still in the flake) Use the commands in all phases (in descending order): 
  - They won't work to the dot since some directories and commands are specific to nix.

## How to use the fresh iso
- As far as I know, the only thing you need to make the iso work is a bootloader that supports the multiboot header (e.x. GRUB).

- On Linux, macOS and BSD, the easiest way is to use qemu: `$ qemu-system-i386 -cdrom result/bin/myos.iso`.
  - If you done it the nix way, using `$ nix develop` installs qemu for you.


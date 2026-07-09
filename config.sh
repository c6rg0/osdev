SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel" # two seperate projects in one string

# Host = target cpu architecture.
# Options: "i686-elf", ...
export HOST=i686
EXECUTABLE_FORMAT=elf

export MAKE=${MAKE:-make}
export AR=${HOST}-${EXECUTABLE_FORMAT}-ar
export AS=${HOST}-${EXECUTABLE_FORMAT}-as
export CC=${HOST}-${EXECUTABLE_FORMAT}-gcc

# The below are related to the kernel headers
export PREFIX=/usr
export EXEC_PREFIX=$PREFIX # IDK why this exists
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

# Compiler flags
export CFLAGS='-O2 -g'
export CPPFLAGS=''

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$EXECUTABLE_FORMAT" | grep -Eq '^elf$'; then
  export CC="$CC -isystem=$INCLUDEDIR"
fi

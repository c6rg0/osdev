#include <stdio.h>

#if defined(__is_libk)
#include <sys/tty.h>
#endif

extern "C" int putchar(int ic)
{
#if defined(__is_libk)
    char c = (char) ic;
    Sys::terminal.write_string(&c);
#else
    // TODO: Implement stdio and the write system call.
#endif
    return ic;
}

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> // putchar(bytes[i]) -> <sys/tty.h> <- tty.c <- vga.h
#include <string.h>

static bool print(const char* data, size_t length)
{
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes [i]) == EOF)
            return false;
    return true;
}

static char* print_hex(uintptr_t value)
{
    static const char digits [] = "0123456789ABCDEF";
    char buffer [8];

    for (int i = 7; i >= 0; i--) {
        buffer [i] = digits [value & 0xF];
        value >>= 4;
    }

    print(buffer, 8);
}

extern "C" int printf(const char* __restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format [0] != '%' || format [1] == '%') {
            if (format [0] == '%')
                format++;
            size_t amount = 1;
            while (format [amount] && format [amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c)))
                return -1;
            written++;

        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(str, len))
                return -1;
            written += len;

        } else if (*format == 'p') {
            format++;
            const uintptr_t ptr = (uintptr_t) va_arg(parameters, void*);

            size_t len = 10;
            if (maxrem < 10) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            const char* hex_sign = "0x";

            if (!print(hex_sign, 2))
                return -1;

            print_hex(ptr);

            written += len;

        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len))
                return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}

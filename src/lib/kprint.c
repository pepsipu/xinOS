#include <lib/cmd.h>
#include <lib/string.h>
#include <stdarg.h>

void kputs(char *str)
{
}

void kprint(char *format, ...)
{
    va_list argp;
    va_start(argp, format);
    for (char *ptr = format; *ptr != '\x00'; ++ptr)
    {
        if (*ptr != '%')
        {
            putc(*ptr, 0xf, 0x0);
            continue;
        }
        switch (*++ptr)
        {
        case 's':
            write_str(va_arg(argp, char *), 0xf, 0x0);
            break;
        case 'd':
            write_str(itoa(va_arg(argp, int), 10), 0xf, 0x0);
            break;
        case 'x':
            write_str(itoa(va_arg(argp, int), 16), 0xf, 0x0);
            break;
        case '%':
            putc('%', 0xf, 0);
            break;
        }
    }
}
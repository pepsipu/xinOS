#include <stdint.h>

uint8_t inline inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("in al, dx"
                 : "=a"(ret)
                 : "d"(port)
                 :);
    return ret;
}

uint16_t inline inw(uint16_t port)
{
    uint16_t ret;
    asm volatile("in ax, dx"
                 : "=a"(ret)
                 : "d"(port)
                 :);
    return ret;
}

uint32_t inline inq(uint16_t port)
{
    uint32_t ret;
    asm volatile("in eax, dx"
                 : "=a"(ret)
                 : "d"(port)
                 :);
    return ret;
}
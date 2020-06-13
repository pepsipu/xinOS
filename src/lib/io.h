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

uint32_t inline ind(uint16_t port)
{
    uint32_t ret;
    asm volatile("in eax, dx"
                 : "=a"(ret)
                 : "d"(port)
                 :);
    return ret;
}

void inline outb(uint16_t port, uint8_t byte)
{
    asm volatile("out dx, al"
                 :
                 : "a"(byte), "d"(port)
                 :);
}

void inline outw(uint16_t port, uint16_t word)
{
    asm volatile("out dx, ax"
                 :
                 : "a"(word), "d"(port)
                 :);
}

void inline outd(uint16_t port, uint32_t dword)
{
    asm volatile("out dx, eax"
                 :
                 : "a"(dword), "d"(port)
                 :);
}
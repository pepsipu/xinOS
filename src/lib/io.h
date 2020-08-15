#include <stdint.h>

#define outb(port, value) ({ asm volatile("out dx, al" : : "a"(value), "d"(port) :); })

#define outw(port, value) ({ asm volatile("out dx, ax" : : "a"(value), "d"(port) :); })

#define outd(port, value) ({ asm volatile("out dx, eax" : : "a"(value), "d"(port) :); })

#define inb(port)                                                                                                      \
    ({                                                                                                                 \
        uint8_t value;                                                                                                 \
        asm volatile("in al, dx" : "=a"(value) : "d"(port) :);                                                         \
        value;                                                                                                         \
    })

#define inw(port)                                                                                                      \
    ({                                                                                                                 \
        uint16_t value;                                                                                                \
        asm volatile("in ax, dx" : "=a"(value) : "d"(port) :);                                                         \
        value;                                                                                                         \
    })

#define ind(port)                                                                                                      \
    ({                                                                                                                 \
        uint32_t value;                                                                                                \
        asm volatile("in eax, dx" : "=a"(value) : "d"(port) :);                                                        \
        value;                                                                                                         \
    })

#define io_wait() outb(0x80, 0)
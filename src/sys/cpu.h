#include <stdint.h>

#define sti() asm volatile("sti")
#define cli() asm volatile("cli")
#define hlt() asm volatile("hlt")
#define hang()                                                                                                         \
    {                                                                                                                  \
        cli();                                                                                                         \
        while (1)                                                                                                      \
            hlt();                                                                                                     \
    }

void cpuid(uint32_t id, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);
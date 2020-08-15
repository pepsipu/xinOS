#include <lib/io.h>
#include <stdint.h>

#define pic_eoi(irq)                                                                                                   \
    ({                                                                                                                 \
        if (irq >= 8)                                                                                                  \
        {                                                                                                              \
            outb(SLAVE_CMD, 0x20);                                                                                     \
        }                                                                                                              \
        outb(MASTER_CMD, 0x20);                                                                                        \
    })

void remap_pic(uint8_t offset);
void init_pic();
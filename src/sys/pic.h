#include <lib/io.h>

#define pic_eoi(irq)                                                                                                   \
    ({                                                                                                                 \
        if (irq >= 8)                                                                                                  \
        {                                                                                                              \
            outb(SLAVE_CMD, 0x20);                                                                                     \
        }                                                                                                              \
        outb(MASTER_CMD, 0x20);                                                                                        \
    })

#include <stdint.h>

#define pcic_read(bus, slot, func, offset, size)                                                                       \
    ((size)(pcic_readd((bus), (slot), (func), (offset)) >> (((offset)&2) * 8)))

uint32_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void init_pci();
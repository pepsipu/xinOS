#include <lib/dynll.h>
#include <stdint.h>

typedef struct
{
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint8_t class;
    uint8_t subclass;
    uint16_t vendor;
    uint16_t device;
} pci_function_t;
new_dynll(pci_functions, pci_function_t);

#define pcic_read(bus, slot, func, offset, size)                                                                       \
    ((size)(pcic_readd((bus), (slot), (func), (offset)) >> (((offset)&2) * 8)))

uint32_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void init_pci();
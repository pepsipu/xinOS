#include <lib/dynll.h>
#include <stdint.h>

#define pcic_address(bus, slot, func, offset)                                                                          \
    (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (offset & 0xfc) |              \
               ((uint32_t)0x80000000))

#define pcic_read(bus, slot, func, offset, size)                                                                       \
    ((size)(pcic_readd((bus), (slot), (func), (offset)) >> (((offset)&2) * 8)))

typedef struct
{
    uint32_t addr;
    uint8_t type;
    uint8_t mem_type;
    uint8_t prefetch;
} pci_bar_t;
typedef struct
{
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint8_t class;
    uint8_t subclass;
    uint16_t vendor;
    uint16_t device;
    pci_bar_t bars[6];
} pci_function_t;

new_type_dynll(pci_functions, pci_function_t);

uint32_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pcic_writed(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);
void enable_pci_bus_mastering(pci_function_t *device);
void init_pci();
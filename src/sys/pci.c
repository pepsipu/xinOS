#include <stdint.h>
#include <lib/io.h>

typedef struct
{

} __attribute__((packed)) config_space_t;

// read word from pci config
uint16_t pcic_readw(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    // calculate address for config_address & ensure enable bit is set (bit 31)
    uint32_t address = (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outd(0xcf8, address);
    // isolate register that was requested
    return (uint16_t)((ind(0xcfc) >> ((offset & 2) * 8)) & 0xffff);
}
uint16_t pci_vendor(uint8_t bus, uint8_t slot)
{
    uint16_t vendor;
    if ((vendor = pcic_readw(bus, slot, 0, 0)) != 0xffff)
    {
    }
    return vendor;
}
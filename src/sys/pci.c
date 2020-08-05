#include <stdint.h>
#include <lib/io.h>
#include <sys/pci.h>

typedef struct
{

} __attribute__((packed)) config_space_t;

typedef struct
{
    uint8_t offset: 8;
    uint8_t func: 3;
    uint8_t device: 5;
    uint8_t bus: 8;
    uint8_t resv: 7;
    uint8_t enable: 1;
} __attribute__((packed)) config_address_t;

typedef union
{
    config_address_t s;
    uint32_t i;
} config_address_u;

// read dword from pci config
uint32_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    // calculate address for config_address & ensure enable bit is set (bit 31)
    // uint32_t address = (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    config_address_u address;
    address.s = (config_address_t){
        offset & 0xfc,
        func,
        slot,
        bus,
        0,
        1
    };
    outd(0xcf8, address.i);
    return ind(0xcfc);
}

uint16_t pci_vendor(uint8_t bus, uint8_t slot)
{
    uint16_t vendor = pcic_readw(bus, slot, 0, 0);
    if (vendor != 0xffff)
    {
    }
    return vendor;
}
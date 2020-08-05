#include <stdint.h>
#include <lib/io.h>
#include <sys/pci.h>
#include <lib/dynll.h>
#include <lib/mem.h>

typedef struct
{

} pci_device_t;

new_dynll(pci_devices, pci_device_t);

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

// read word from pci config
uint16_t pcic_readw(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
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
    return (uint16_t)((ind(0xcfc) >> ((offset & 2) * 8)) & 0xffff);
}

uint16_t pci_vendor(uint8_t bus, uint8_t slot)
{
    uint16_t vendor = pcic_readw(bus, slot, 0, 0);
    if (vendor != 0xffff)
    {
    }
    return vendor;
}
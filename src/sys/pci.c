#include <lib/dynll.h>
#include <lib/io.h>
#include <lib/mem.h>
#include <stdint.h>
#include <sys/pci.h>

typedef struct
{
} pci_slot_t;

new_dynll(pci_slots, pci_slot_t);

// typedef struct
// {
//     uint8_t offset: 8;
//     uint8_t func: 3;
//     uint8_t slot: 5;
//     uint8_t bus: 8;
//     uint8_t resv: 7;
//     uint8_t enable: 1;
// } __attribute__((packed)) config_address_t;

// typedef union
// {
//     config_address_t s;
//     uint32_t i;
// } config_address_u;

// read dword from pci config
uint32_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    // calculate address for config_address & ensure enable bit is set (bit 31)
    uint32_t address = (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) |
                                  (offset & 0xfc) | ((uint32_t)0x80000000));
    // config_address_u address;
    // address.s = (config_address_t){
    //     offset & 0xfc,
    //     func,
    //     slot,
    //     bus,
    //     0,
    //     1
    // };
    outd(0xcf8, address);
    return ind(0xcfc);
}

// uint16_t pcic_readw(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
// {
//     return (uint16_t)(pcic_readd(bus, slot, func, offset) >> ((offset & 2) *
//     8));
// }

// given a bus, recusively scan for secondary buses to enumerate all possible
// buses
void enumerate_pci_slots_from_bus(uint8_t bus)
{
    for (uint8_t slot = 0; slot < 32; ++slot)
    {
        uint16_t vendor = pcic_read(bus, slot, 0, 0, uint16_t);
        if (vendor == 0xffff)
            continue;
    }
}

void enumerate_pci_slots()
{
    // read first pci controller header
    uint8_t header = pcic_read(0, 0, 0, 7, uint8_t);
    enumerate_pci_slots_from_bus(0);
    if (header & 0x80)
    {
        for (uint8_t func = 1; func < 8; ++func)
        {
            uint16_t vendor = pcic_read(0, 0, func, 0, uint16_t);
            // make sure vendor is invalid
            if (vendor != 0xffff)
                break;
            enumerate_pci_slots_from_bus(func);
        }
    }
}
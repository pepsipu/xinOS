#include <lib/kprint.h>
#include <lib/mem/dynll.h>
#include <lib/mem/mem.h>
#include <stdint.h>
#include <sys/io.h>
#include <sys/pci.h>

new_dynll(pci_functions);

// read dword from pci config

uint32_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    // calculate address for config_address & ensure enable bit is set (bit 31)
    uint32_t address = pcic_address(bus, slot, func, offset);
    outd(0xcf8, address);
    return ind(0xcfc);
}

void pcic_writed(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value)
{
    uint32_t address = pcic_address(bus, slot, func, offset);
    outd(0xcf8, address);
    outd(0xcfc, value);
}

// given a bus, recusively scan for secondary buses to enumerate all possible
// buses
void enumerate_pci_slots_from_bus(uint8_t bus)
{
    for (uint8_t slot = 0; slot < 32; ++slot)
    {
        uint16_t vendor = pcic_read(bus, slot, 0, 0, uint16_t);
        if (vendor == 0xffff)
            continue;
        handle_pci_function(bus, slot, 0);
        uint8_t header = pcic_read(bus, slot, 0, 14, uint8_t);
        if (header & 0x80)
        {
            for (uint8_t func = 1; func < 8; ++func)
            {
                if (pcic_read(bus, slot, func, 0, uint16_t) != 0xffff)
                {
                    handle_pci_function(bus, slot, func);
                }
            }
        }
    }
}

void enumerate_pci_slots()
{
    // read first pci controller header
    uint8_t header = pcic_read(0, 0, 0, 14, uint8_t);
    enumerate_pci_slots_from_bus(0);
    if (header & 0x80)
    {
        for (uint8_t func = 1; func < 8; ++func)
        {
            uint16_t vendor = pcic_read(0, 0, func, 0, uint16_t);
            // make sure vendor is valid
            if (vendor == 0xffff)
                continue;
            enumerate_pci_slots_from_bus(func);
        }
    }
}

void handle_pci_function(uint8_t bus, uint8_t slot, uint8_t func)
{
    uint8_t class = pcic_read(bus, slot, func, 11, uint8_t);
    uint8_t subclass = pcic_read(bus, slot, func, 10, uint8_t);
    // ensure class is bridge and subclass is pci to pci
    // if this function is connected to pci, then we can enumerate another bus
    if (class == 6 && subclass == 4)
    {
        kprint("found pci bridge on bus %x slot %x with secondary bus of %x", bus, slot,
               pcic_read(bus, slot, func, 19, uint8_t));
        enumerate_pci_slots_from_bus(pcic_read(bus, slot, func, 19, uint8_t));
    }
    append_dynll(pci_functions, ((pci_function_t){.bus = bus,
                                                  .slot = slot,
                                                  .func = func,
                                                  .class = class,
                                                  .subclass = subclass,
                                                  .vendor = pcic_read(bus, slot, func, 0, uint16_t),
                                                  .device = pcic_read(bus, slot, func, 2, uint16_t)}));
}

// void print_devices_debug(pci_function_t *ptr)
// {
//     kprint("Bus: %x Slot: %x Func: %x Class: %x Subclass: %x Vendor: %x Device: %x\n", ptr->bus, ptr->slot,
//     ptr->func,
//            ptr->class, ptr->subclass, ptr->vendor, ptr->device);
// }

void init_pci()
{
    enumerate_pci_slots();
    // iter_dynll(pci_functions, print_devices_debug);
}

void read_pci_bar(pci_function_t *device, uint8_t bar_idx)
{
    pci_bar_t *bar_device = &device->bars[bar_idx];
    uint32_t bar = pcic_read(device->bus, device->slot, device->func, 16 + bar_idx * 4, uint32_t);
    bar_device->type = bar & 1;
    if (bar_device->type)
    {
        // this is an io space bar
        bar_device->addr = bar & 0xfffffffc;
    }
    else
    {
        // this is a mem space bar
        // for the sake of my sanity ignore 64 bit bars
        bar_device->mem_type = (bar & 0xe) >> 1;
        bar_device->prefetch = (bar & 0x7) >> 3;
        bar_device->addr = bar & 0xfffffff0;
    }
}

void enable_pci_bus_mastering(pci_function_t *device)
{
    pcic_writed(device->bus, device->slot, device->func, 4,
                pcic_readd(device->bus, device->slot, device->func, 4) | (1 << 2));
}

void get_interrupt_line(pci_function_t *device)
{
    device->interrupt_line = pcic_read(device->bus, device->slot, device->func, 0x3c, uint8_t);
}
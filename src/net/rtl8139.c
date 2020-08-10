#include <lib/dynll.h>
#include <lib/io.h>
#include <lib/kprint.h>
#include <net/rtl8139.h>
#include <sys/pci.h>

#define RT_VENDOR 0x10ec
#define RT_DEVICE 0x8139

pci_function_t *pci_rtl8139 = 0;
uint8_t rx_buf[8208];

pci_function_t *get_rtl8139()
{
    if (!pci_rtl8139)
        return search_dynll(pci_functions, element->device == RT_DEVICE && element->vendor == RT_VENDOR);
    else
        return pci_rtl8139;
}

int init_rtl8139()
{
    pci_rtl8139 = get_rtl8139();
    enable_pci_bus_mastering(pci_rtl8139);
    read_pci_bar(pci_rtl8139, 0);
    uint32_t io_offset = pci_rtl8139->bars[0].addr;
    // power on rtl
    outb(io_offset + 0x52, 0);
    // clear tx & rx with software reset
    outb(io_offset + 0x37, 0x10);
    // wait for software reset to finish
    while (!(inb(io_offset + 0x37) & 0x10))
    {
    }
    // configure rx buffer
    outd(io_offset + 0x30, rx_buf);
    if (!pci_rtl8139)
    {
        return 1;
    }
}
#include <lib/dynll.h>
#include <lib/io.h>
#include <lib/kprint.h>
#include <net/rtl8139.h>
#include <sys/pci.h>

#define RT_VENDOR 0x10ec
#define RT_DEVICE 0x8139

#define rtl_wb(reg, val) outb(io_offset + (reg), val)
#define rtl_ww(reg, val) outw(io_offset + (reg), val)
#define rtl_wd(reg, val) outd(io_offset + (reg), val)

#define rtl_rb(reg) inb(io_offset + (reg))
#define rtl_rw(reg) inw(io_offset + (reg))
#define rtl_rd(reg) ind(io_offset + (reg))

enum rtl_regs
{
    CONFIG0 = 0x51,
    CR = 0x37,
    RBSTART = 0x30,
};

enum cmd_bits
{
    CMD_RST = (1 << 4),
};

struct
{
    uint64_t mac;
} nic;

pci_function_t *pci_rtl8139 = 0;
uint32_t io_offset;
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
    if (!pci_rtl8139)
    {
        return 1;
    }
    enable_pci_bus_mastering(pci_rtl8139);
    read_pci_bar(pci_rtl8139, 0);
    io_offset = pci_rtl8139->bars[0].addr;
    // set lwake & lwptn high to power on device
    rtl_wb(CONFIG0, 0);
    // clear tx & rx with software reset
    rtl_wb(CR, CMD_RST);
    // wait for software reset to finish
    while (!(rtl_rb(CR) & CMD_RST))
    {
    }
    // configure rx buffer
    rtl_wd(RBSTART, rx_buf);
}
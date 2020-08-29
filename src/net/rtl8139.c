#include <lib/kprint.h>
#include <lib/mem/dynll.h>
#include <net/rtl8139.h>
#include <sys/idt.h>
#include <sys/io.h>
#include <sys/pci.h>
#include <sys/pic.h>

#define RT_VENDOR 0x10ec
#define RT_DEVICE 0x8139

#define rtl_wb(reg, val) outb(io_offset + (reg), (val))
#define rtl_ww(reg, val) outw(io_offset + (reg), (val))
#define rtl_wd(reg, val) outd(io_offset + (reg), (val))

#define rtl_rb(reg) inb(io_offset + (reg))
#define rtl_rw(reg) inw(io_offset + (reg))
#define rtl_rd(reg) ind(io_offset + (reg))

enum rtl_regs
{
    IDR1 = 0x0,
    IDR4 = 0x4,
    CONFIG1 = 0x52,
    CR = 0x37,
    RBSTART = 0x30,
    RCR = 0x44,
};

enum cmd_bits
{
    CMD_TE = 1 << 2,
    CMD_RE = 1 << 3,
    CMD_RST = 1 << 4,
};

enum rx_config_bits
{
    RXC_AAP = 1 << 0,
    RXC_APM = 1 << 1,
    RXC_AM = 1 << 2,
    RXC_AB = 1 << 3,
};

struct
{
    uint64_t mac;
} nic = {0};

pci_function_t *pci_rtl8139 = 0;
uint32_t io_offset = 0;
uint8_t rx_buf[8208] = {0};

pci_function_t *get_rtl8139()
{
    if (!pci_rtl8139)
        return search_dynll(pci_functions, element->device == RT_DEVICE && element->vendor == RT_VENDOR);
    else
        return pci_rtl8139;
}

void rtl_interrupt()
{
    kprint("epic interrupt for rtl");
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
    rtl_wb(CONFIG1, 0);
    // clear tx & rx with software reset
    rtl_wb(CR, CMD_RST);
    // wait for software reset to finish
    while (rtl_rb(CR) & CMD_RST)
    {
    }
    nic.mac = (uint64_t)rtl_rd(IDR1) | (((uint64_t)rtl_rw(IDR4)) << 32);
    get_interrupt_line(pci_rtl8139);
    register_isr(rtl_interrupt, PIC_OFFSET + pci_rtl8139->interrupt_line);
    // set rx buffer
    rtl_wd(RBSTART, rx_buf);
    // configure rx to take all, broadcast, multicast, & physical packets
    rtl_wd(RCR, RXC_AAP | RXC_AB | RXC_AM | RXC_APM);
    // enable rx and tx
    rtl_wb(CR, CMD_RE | CMD_TE);
}
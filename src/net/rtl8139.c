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

// i got these enums for regs/cmds from linux drivers for rtl813x
enum rtl_regs
{
    MAC0 = 0,         /* Ethernet hardware address. */
    MAR0 = 8,         /* Multicast filter. */
    TxStatus0 = 0x10, /* Transmit status (Four 32bit registers). */
    TxAddr0 = 0x20,   /* Tx descriptors (also four 32bit). */
    RxBuf = 0x30,
    ChipCmd = 0x37,
    RxBufPtr = 0x38,
    RxBufAddr = 0x3A,
    IntrMask = 0x3C,
    IntrStatus = 0x3E,
    TxConfig = 0x40,
    RxConfig = 0x44,
    Timer = 0x48,    /* A general-purpose counter. */
    RxMissed = 0x4C, /* 24 bits valid, write clears. */
    Cfg9346 = 0x50,
    Config0 = 0x51,
    Config1 = 0x52,
    TimerInt = 0x54,
    MediaStatus = 0x58,
    Config3 = 0x59,
    Config4 = 0x5A, /* absent on RTL-8139A */
    HltClk = 0x5B,
    MultiIntr = 0x5C,
    TxSummary = 0x60,
    BasicModeCtrl = 0x62,
    BasicModeStatus = 0x64,
    NWayAdvert = 0x66,
    NWayLPAR = 0x68,
    NWayExpansion = 0x6A,
    /* Undocumented registers, but required for proper operation. */
    FIFOTMS = 0x70, /* FIFO Control and test. */
    CSCR = 0x74,    /* Chip Status and Configuration Register. */
    PARA78 = 0x78,
    FlashReg = 0xD4, /* Communication with Flash ROM, four bytes. */
    PARA7c = 0x7c,   /* Magic transceiver parameter register. */
    Config5 = 0xD8,  /* absent on RTL-8139A */
};

enum rtl_cmd_bits
{
    CmdReset = 0x10,
    CmdRxEnb = 0x08,
    CmdTxEnb = 0x04,
    RxBufEmpty = 0x01,
};

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
    enable_pci_bus_mastering(pci_rtl8139);
    read_pci_bar(pci_rtl8139, 0);
    io_offset = pci_rtl8139->bars[0].addr;
    // set lwake & lwptn high to power on device
    rtl_wb(Config0, 0);
    // clear tx & rx with software reset
    rtl_wb(ChipCmd, CmdReset);
    // wait for software reset to finish
    while (!(rtl_rb(ChipCmd) & CmdReset))
    {
    }
    // configure rx buffer
    rtl_wd(RxBuf, rx_buf);
    if (!pci_rtl8139)
    {
        return 1;
    }
}
#include <lib/io.h>
#include <lib/kprint.h>
#include <sys/pic.h>

enum pic_regs
{
    MASTER_CMD = 0x20,
    MASTER_DATA = 0x21,
    SLAVE_CMD = 0xa0,
    SLAVE_DATA = 0xa1,
};

enum icw1_bits
{
    ICW1_PREP_IC4 = 1 << 0,
    ICW1_INIT = 1 << 4,
};

void remap_pic(uint8_t offset)
{
    // stage 1: start init and tell pic to prep for 4th stage
    outb(MASTER_CMD, ICW1_INIT | ICW1_PREP_IC4);
    io_wait();
    outb(SLAVE_CMD, ICW1_INIT | ICW1_PREP_IC4);
    io_wait();

    // stage 2: give offsets to master & slave
    outb(MASTER_DATA, offset);
    io_wait();
    outb(SLAVE_DATA, offset + 8);
    io_wait();

    // stage 3: setup master & slave comm irq lines
    // IRQ2's bit pos is 2, so set bit 2 (4 is IRQ2)
    outb(MASTER_DATA, 4);
    io_wait();
    // IRQ2 in binary (an actual 2)
    outb(SLAVE_DATA, 2);
    io_wait();

    // set 80x86 mode since we are working on x86
    outb(MASTER_DATA, 1);
    io_wait();
    outb(SLAVE_DATA, 1);
    io_wait();

    // set interrupt masks so no interrupts by default
    outb(MASTER_DATA, 0xff);
    outb(SLAVE_DATA, 0xff);
}

void init_pic()
{
    // good place so it wont conflict with cpu faults
    remap_pic(0x20);
}

void set_imr(uint8_t irq)
{
    uint8_t port;
    if (irq < 8)
    {
        port = MASTER_DATA;
    }
    else
    {
        port = SLAVE_DATA;
        irq -= 8;
    }
    outb(port, inb(port) | (1 << irq));
}

void clear_imr(uint8_t irq)
{
    uint8_t port;
    if (irq < 8)
    {
        port = MASTER_DATA;
    }
    else
    {
        port = SLAVE_DATA;
        irq -= 8;
    }
    outb(port, inb(port) & ~(1 << irq));
}
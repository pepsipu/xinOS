#include <boot/multiboot.h>
#include <lib/cmd.h>
#include <lib/kprint.h>
#include <lib/mem/mem.h>
#include <mm/mm.h>
#include <net/rtl8139.h>
#include <sys/cpu.h>
#include <sys/idt.h>
#include <sys/pci.h>
#include <sys/pic.h>

void kmain(multiboot_info_t *mb)
{
    init_cmd();
    init_idt();
    init_pic();
    init_pmm(mb->mmap_addr, mb->mmap_length);
    init_pci();
    init_rtl8139();
    sti();
    while (1)
    {
    }
}

#include <boot/multiboot.h>
#include <lib/cmd.h>
#include <lib/kprint.h>
#include <lib/mem.h>
#include <mm/mm.h>
#include <net/rtl8139.h>
#include <sys/idt.h>
#include <sys/pci.h>

multiboot_info_t mb;

void kmain(multiboot_info_t *mb_ptr)
{
    // copy multiboot information so we don't need to deref a pointer on access & prevent possible clobbering
    memcpy(&mb, mb_ptr, sizeof(multiboot_info_t));
    init_cmd();
    init_idt();
    init_pmm(mb.mmap_addr, mb.mmap_length);
    init_pci();
    init_rtl8139();
}
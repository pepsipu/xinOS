#include <boot/multiboot.h>
#include <lib/mem.h>
#include <mm/mm.h>
#include <sys/idt.h>
#include <lib/kprint.h>
#include <sys/apic.h>
#include <lib/cmd.h>
#include <sys/pci.h>

multiboot_info_t mb;

void kmain(multiboot_info_t *mb_ptr)
{
    // copy multiboot information so we don't need to deref a pointer every time we need to access it & prevent possible clobbering
    memcpy(&mb, mb_ptr, sizeof(multiboot_info_t));
    init_cmd();
    init_idt();
    if (init_apic()) {
        kprint("your system is far too boomer to support apic, get a better pc lul");
    }
    init_pmm(mb.mmap_addr, mb.mmap_length);
    kprint("test vendor id: %x", pcic_readw(0,0,0,0));
}
#include <boot/multiboot.h>
#include <lib/mem.h>
#include <mm/mm.h>

multiboot_info_t mb;

void kmain(multiboot_info_t *mb_ptr) {
    // copy multiboot information so we don't need to deref a pointer every time we need to access it.
    memcpy(&mb, mb_ptr, sizeof(multiboot_info_t));
    init_pmm(mb.mmap_addr, mb.mmap_length);
}
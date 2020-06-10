#include <boot/multiboot.h>
#include <stdint.h>

void init_pmm(multiboot_memory_map_t *mmap_addr, uint32_t mmap_length) {
    multiboot_memory_map_t *entry = mmap_addr;
    // use dynamic stack array because we don't have heap yet
    multiboot_memory_map_t *free_memory_slots[mmap_length / sizeof(multiboot_memory_map_t)];
    for (int i = 0; mmap_addr + mmap_length > entry;) {
        if (entry->type == 1) {
            free_memory_slots[i] = entry;
            i++;
        }
        entry = (multiboot_memory_map_t *) ((uint32_t) entry + entry->size);
    }
}
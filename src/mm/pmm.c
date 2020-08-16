#include <boot/multiboot.h>
#include <lib/mem.h>
#include <stddef.h>
#include <stdint.h>

struct free_chunk
{
    size_t size;
    struct free_chunk *fd;
    struct free_chunk *bk;
};

typedef struct free_chunk free_chunk_t;

typedef struct
{
    size_t size;
    uint8_t data[];
} allocated_chunk_t;

typedef struct
{
    void *addr;
    uint64_t len;
    uint64_t used;
} region_state_t;

region_state_t *fm_regions;
uint32_t fm_region_len;
free_chunk_t free_bin = (free_chunk_t){
    .size = 0,
    .fd = &free_bin,
    .bk = &free_bin,
};

void init_pmm(multiboot_memory_map_t *mmap_addr, uint32_t mmap_length)
{
    multiboot_memory_map_t *entry = mmap_addr;
    // use dynamic stack array because we don't have heap yet
    multiboot_memory_map_t *free_memory_region[mmap_length / sizeof(multiboot_memory_map_t)];
    int free_memory_count;
    for (free_memory_count = 0; mmap_addr + mmap_length > entry;)
    {
        if (entry->type == 1)
        {
            free_memory_region[free_memory_count] = entry;
            ++free_memory_count;
        }
        entry = (multiboot_memory_map_t *)((uint32_t)entry + entry->size + sizeof(entry->size));
    }
    fm_region_len = free_memory_count;
    // find region who can store metadata
    int metadata_chunk_index = 0;
    size_t size_needed = sizeof(region_state_t) * fm_region_len;
    while (metadata_chunk_index < fm_region_len)
    {
        multiboot_memory_map_t *region = free_memory_region[metadata_chunk_index];
        if (region->len > size_needed)
        {
            fm_regions = region->addr;
            // shift over usable memory so we dont overwrite the region metadata
            region->addr += size_needed;
            for (int i = 0; fm_region_len > i; ++i)
            {
                region_state_t *new_region = fm_regions + i * sizeof(region_state_t);
                *new_region = (region_state_t){
                    .addr = region->addr,
                    .len = region->len,
                    .used = 0,
                };
            }
            break;
        }
        ++metadata_chunk_index;
    }
}

// check for free chunk with same size -> check for chunk to split -> allocate new chunk
void *kmalloc(size_t requested_size)
{
    if (requested_size == 0)
        return NULL;
    // round to nearest multiple of 8 so we get alignment
    size_t aligned_size = (requested_size + sizeof(allocated_chunk_t) + 7) & -8;
    // ensure size is big enough to fit a free chunk
    size_t size = aligned_size < sizeof(free_chunk_t) ? sizeof(free_chunk_t) : aligned_size;
    free_chunk_t *ptr = free_bin.fd;
    while (ptr != &free_bin)
    {
        if (ptr->size == size)
        {
            // unlink from free list
            ptr->bk->fd = ptr->fd;
            ptr->fd->bk = ptr->bk;
            return ptr + sizeof(allocated_chunk_t);
        }
        // ensure we can split chunk and still have space for a free chunk
        if (ptr->size > size && ptr->size - size - sizeof(allocated_chunk_t) > sizeof(free_chunk_t))
        {
            ptr->size -= size + sizeof(allocated_chunk_t);
            allocated_chunk_t *new_chunk = ptr + ptr->size + sizeof(allocated_chunk_t);
            new_chunk->size = size;
        }
        ptr = ptr->fd;
    }
    // if we can't find a free chunk, we can make a new chunk from the memory regions
    for (int i = 0; i < fm_region_len; i++)
    {
        region_state_t *region = &fm_regions[i];
        if (region->len - region->used > size)
        {
            allocated_chunk_t *new_chunk = region->addr + region->used;
            new_chunk->size = size;
            region->used += size + sizeof(allocated_chunk_t);
            return &new_chunk->data;
        }
    }
    // uh oh
    return NULL;
}

void kfree(void *chunk)
{
    free_chunk_t *chunk_to_free = chunk - sizeof(allocated_chunk_t);
    // link chunk to free list
    chunk_to_free->bk = free_bin.fd->bk;
    free_bin.fd->bk = chunk_to_free;
    chunk_to_free->fd = free_bin.fd;
    free_bin.fd = chunk_to_free;
}

void *krealloc(void *chunk, size_t new_size)
{
    void *new_chunk = kmalloc(new_size);
    size_t old_size = *(size_t *)(chunk - sizeof(allocated_chunk_t));
    memcpy(new_chunk, chunk, old_size > new_size ? old_size - new_size : new_size);
    kfree(chunk);
    return new_chunk;
}
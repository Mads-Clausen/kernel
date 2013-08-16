#include <multiboot.h>

void get_multiboot_info(struct multiboot_info *mbt, uint64_t *bytes)
{
    memory_map_t *mmap = (memory_map_t *) mbt->mmap_addr;
    int i = 0;
    while((uint32_t) mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        if(mmap->type == 1)
            *bytes +=   (uint64_t) (mmap->length_low    | (uint64_t) (mmap->length_high) << 32) +
                        (uint64_t) (mmap->base_addr_low | (uint64_t) (mmap->base_addr_high) << 32);
        mmap = (memory_map_t *) ((unsigned int) mmap + mmap->size + sizeof(mmap->size));
    }
}
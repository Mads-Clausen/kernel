#include <mem.h>

uint32_t *page_directory = (uint32_t *) 0x9000;
uint32_t *page_table     = (uint32_t *) 0xA000;

void init_paging()
{
    uint32_t address = 0;
    uint32_t i;

    /* map first 4 MiB 1:1 */
    for(i = 0; i < 1024; ++i)
    {
        page_table[i] = (address & ~(0xFFF)) | 3; /* attribute set to: supervisor level, read/write, present(011 in binary) */
        address = (uint32_t) (address + 0x1000);
    }

    page_directory[0]  = (uint32_t) page_table;
    page_directory[0] |= 3;
    /* kprintf("page_directory[0] = %x\n", (uint64_t) page_directory[0]); */

    for(i = 1; i < 1024; ++i)
    {
        page_directory[i] = 0;
    }

    extern void     _write_cr3(uint32_t);
    extern void     _write_cr0(uint32_t);
    extern uint32_t _read_cr0();

    _write_cr3((uint32_t) page_directory);
    _write_cr0(_read_cr0() | 0x80000000); /* set the paging bit in CR0 to 1 */
}

/*
void map_page(uint32_t virtual_addr, uint64_t real_addr)
{

    virtual_addr /= 0x1000;
    *((uint32_t *)(page_directory[virtual_addr / 1024]) + virtual_addr % 1024) = (real_addr & ~(0xFFF)) | 3;
}
*/

extern void _flush_tlb();

/* NOTE: VIRTUAL ADDRESSES ARE LIMITED TO 0xF0F00000 */
void map_page(uint32_t virtual_addr, uint32_t real_addr, unsigned int flags)
{
    uint32_t pdindex = virtual_addr >> 22;
    uint32_t ptindex = virtual_addr >> 12 & 0x3FF;

    uint32_t *pt = (page_table) + (1024 * pdindex);
    
    /*
    kprintf("pdindex = %u. Content = 0x%x. pt = 0x%x\n", (uint64_t) pdindex, 
                                (uint64_t) page_directory[pdindex] & 0x01, (uint64_t) (uint32_t) pt); /**/

    if(page_directory[pdindex] == 0) /* it isn't present, create a new one */
    {
        uint32_t i;
        for(i = 0; i < 1024; ++i)
        {
            pt[i] = 3; /* attribute set to: supervisor level, read/write, present(011 in binary) */
        }

        /*
        kprintf("PD loc: 0x%x\n", (uint64_t) (uint32_t) page_directory + pdindex); /**/
        page_directory[pdindex] = (uint32_t) pt;
        page_directory[pdindex] |= 3;
        
        /*
        kprintf("Content of PD after if = 0x%x\n", (uint64_t) page_directory[pdindex]); /**/
        _flush_tlb();
    }
 
    /*
    kprintf("Real address = 0x%x\n", (uint64_t) ((uint32_t) real_addr) | (flags & 0xFFF) | 1);
    kprintf("ptindex = %u\n", (uint64_t) ptindex); /**/
    pt[ptindex] = 0;
    pt[ptindex] = ((uint32_t) real_addr) | (flags & 0xFFF) | 1;
    /*
    kprintf("Content of PT after if = 0x%x, 0x%x\n", (uint64_t) pt[ptindex], 
                                (uint64_t) ((uint32_t) real_addr) | (flags & 0xFFF) | 1); /**/
}

uint32_t real_addr(uint32_t virtual_addr)
{
    uint32_t entry = virtual_addr >> 12;

    if(*(page_directory + entry) & 0x01)
        return (*(page_directory + entry) >> 12) & ~(0xFFF);

    return 0;
}

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
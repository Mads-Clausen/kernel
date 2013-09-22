#include <mem.h>

uint32_t *page_directory = (uint32_t *) 0x9000;
uint32_t *page_table     = (uint32_t *) 0xA000;
uint32_t *bitmap;

extern void     _write_cr3(uint32_t);
extern void     _write_cr0(uint32_t);
extern uint32_t _read_cr0();

void init_paging()
{
    uint32_t address = 0;
    uint32_t i;

    /* map first 4 MiB 1:1 */
    for(i = 0; i < PT_LENGTH; ++i)
    {
        page_table[i] = (address & ~(0xFFF)) | 3; /* attribute set to: supervisor level, read/write, present(011 in binary) */
        address = (uint32_t) (address + 0x1000);
    }

    page_directory[0]  = (uint32_t) page_table;
    page_directory[0] |= 3;

    for(i = 1; i < PD_LENGTH; ++i)
    {
        page_directory[i] = 0;
    }

    /* allocate space for the header */
    uint32_t *header = page_table + PT_LENGTH;
    bitmap = (uint32_t *) address;
    for(i = 0; i < PT_LENGTH; ++i)
    {
        header[i] = (address & ~(0xFFF)) | 3; /* attribute set to: supervisor level, read/write, present(011 in binary) */
        address = (uint32_t) (address + 0x1000);
    }

    /* set all pages to present in the header */
    for(i = 0; i < PD_LENGTH * PT_LENGTH; ++i)
    {
        bitmap[i] = 1; /* present */
    }

    for(i = 0; i < 2 * 1024; ++i)
        bitmap[i] = 3; /* present, in use, process ID 0(kernel) */

    page_directory[1]  = (uint32_t) header;
    page_directory[1] |= 3;

    _write_cr3((uint32_t) page_directory);
    _write_cr0(_read_cr0() | 0x80000000); /* set the paging bit in CR0 to 1 */
}

extern void _flush_tlb();

void map_page(uint32_t virtual_addr, uint32_t real_addr, unsigned int flags)
{
    uint32_t pdindex = virtual_addr >> 22;
    uint32_t ptindex = virtual_addr >> 12 & 0x3FF;

    uint32_t *pt = (page_table) + (PT_LENGTH * pdindex);

    if((page_directory[pdindex] & 1) == 0) /* it isn't present, create a new one */
    {
        uint32_t i;
        for(i = 0; i < PT_LENGTH; ++i)
            pt[i] = 3; /* attribute set to: supervisor level, read/write, present(011 in binary) */

        page_directory[pdindex] = (uint32_t) pt;
        page_directory[pdindex] |= 3;

        _flush_tlb();
    }

    pt[ptindex] = 0;
    pt[ptindex] = ((uint32_t) real_addr) | (flags & 0xFFF) | 1;
}

uint32_t real_addr(uint32_t virtual_addr)
{
    uint32_t entry = virtual_addr >> 12;

    if(*(page_directory + entry) & 0x01)
        return (*(page_directory + entry) >> 12) & ~(0xFFF);

    return 0;
}

uint32_t allocate_page(uint16_t proc_id)
{
    unsigned int i;

    for(i = 0; i < PD_LENGTH * PT_LENGTH; ++i)
    {
        if((bitmap[i] & 1) && !(bitmap[i] & 2))
        {
            bitmap[i] |= (proc_id << 16) | 2; /* put the process ID into the top 16 bits */
            break;
        }
    }

    uint32_t
        pti = i - (i / 1024) * 1024,
        pdi = i / 1024;

    return pdi * 0x400000 + pti * 0x1000;
}

void set_page_directory(uint32_t *pd)
{
    page_directory = pd;
    _write_cr3((uint32_t) page_directory);
    _flush_tlb();
}

void print_next_available_page()
{
    unsigned int i;

    for(i = 0; i < PD_LENGTH * PT_LENGTH; ++i)
    {
        if((bitmap[i] & 1) && !(bitmap[i] & 2))
        {
            kprintf("Next available page:\nPTI: %i\nPDI: %i\n", i - (i / 1024) * 1024, i / 1024);
            break;
        }
    }
}
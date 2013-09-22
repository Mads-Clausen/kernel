#ifndef __MEM_H_INCLUDED__
#define __MEM_H_INCLUDED__

#include <stdint.h>
#include <system.h>
#include <multiboot.h>

#define PD_LENGTH 1024
#define PT_LENGTH 1024

struct page_descriptor
{
    int pdindex, ptindex;
};

/** Initialises paging */
void init_paging(void);

/** Map a virtual 4K-aligned address to a physical 4K-aligned address */
void map_page(uint32_t virtual_addr, uint32_t real_addr, unsigned int flags);

/** Returns the physical address of a virtual address */
uint32_t real_addr(uint32_t virtual_addr);

/** Allocates a page to a process */
uint32_t allocate_page(uint16_t proc_id);

/** Prints information about the next available page */
void print_next_available_page();

/** Sets the page directory pointer */
void set_page_directory(uint32_t *pd);

/** Allocates x amount of bytes */
void *malloc(uint32_t size);

/** Frees allocated memory */
void free(void *ptr);

#endif
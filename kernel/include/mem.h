#ifndef __MEM_H_INCLUDED__
#define __MEM_H_INCLUDED__

#include <stdint.h>
#include <system.h>
#include <multiboot.h>

void init_paging(void);
// void map_page(uint32_t virtual_addr, uint64_t real_addr);
void map_page(uint32_t virtual_addr, uint32_t real_addr, unsigned int flags);
uint32_t real_addr(uint32_t virtual_addr);

void get_multiboot_info(struct multiboot_info *mbt, uint64_t *bytes);

#endif
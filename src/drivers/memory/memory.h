/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _MEMORY_H
#define _MEMORY_H 1

#include "../../boot/multiboot.h"

typedef struct page_entry {
    uint8_t type;
    uint8_t state; 
	uint64_t start;
    // page_entry_t* next;
} __attribute__ ((packed)) page_entry_t;

extern void paging_initialize_from_multiboot(const multiboot_info_t* mbt);
//extern void add_page(uint8_t type, uint8_t state, uint64_t start);

#endif
/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stddef.h>
#include <stdint.h>
#include "liballoc_1_1.h"
#include "memory.h"
#include "../terminal/terminal.h"

#define PAGESIZE 1024
#define MAX_PAGES 1024 * 1024

static page_entry_t PAGES[MAX_PAGES];

// static page_entry_t* lastPage = NULL;
static uint32_t currentMaxPageNo = 0;
static uint32_t totalPagesAdded = 0;

void paging_initialize_from_multiboot(const multiboot_info_t* mbt)
{
    int terminal = 0;
    multiboot_memory_map_t* mmap = mbt->mmap_addr;
    int i = 0;

	while(mmap < mbt->mmap_addr + mbt->mmap_length) {
        
        for(uint64_t j = mmap->base_addr; j < (mmap->length + mmap->base_addr);j+=PAGESIZE) {
            PAGES[totalPagesAdded].start = j;
            PAGES[totalPagesAdded].state = 0;
            PAGES[totalPagesAdded++].type = 0;
        }
        
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
        i++;        
	}
    
    terminal_putdec(totalPagesAdded);
    fputs(" total pages added.", terminal);
    terminal_newline();

    uint64_t test = liballoc_alloc(3);
    
    if(test < 0) {
        fputs("alloc returned null", terminal);
    } else {
        // while(1){}
        terminal_putdec(test);
        fputs(" adr of alloc.", terminal);
        terminal_newline();
        
    }

    uint64_t test2 = liballoc_alloc(3);
    
    if(test2 < 0) {
        fputs("alloc returned null", terminal);
    } else {
        // while(1){}
        terminal_putdec(test2);
        fputs(" adr of alloc2.", terminal);
        terminal_newline();
        
    }

    liballoc_free(test, 3);
    
    uint64_t test3 = liballoc_alloc(4);
    
    if(test3 < 0) {
        fputs("alloc returned null", terminal);
    } else {
        // while(1){}
        terminal_putdec(test3);
        fputs(" adr of alloc3.", terminal);
        terminal_newline();
        
    }
}

int liballoc_lock()
{
	//pthread_mutex_lock( &mutex );
	return 0;
}

int liballoc_unlock()
{
	//pthread_mutex_unlock( &mutex );
	return 0;
}

void* liballoc_alloc( size_t pages )
{	
    int allocated = 0;
    int lastFreePage = -1;

    for(int i = 0; (i < totalPagesAdded) && (allocated < pages); i++) {
        if(PAGES[i].state == 0) {
            allocated++;
            if(lastFreePage == -1) {
                lastFreePage = i;
            }
        } else {
            allocated = 0;
            lastFreePage = -1;
        }
    }
    
    if((allocated < pages) || (lastFreePage < 0) || ((lastFreePage + pages) > totalPagesAdded)) {
        return -1;
    }
    for(int j=lastFreePage; j < (lastFreePage + pages); j++) {
        PAGES[j].state = 1;
    }

	return PAGES[lastFreePage].start;
}

int liballoc_free( void* ptr, size_t pages )
{
	for(int i = 0; i < totalPagesAdded; i++) {
        if(PAGES[i].start == (uint64_t)ptr) {
            for(int j = i; j < i + pages; j++) {
                PAGES[j].state = 0;
            }
            return 0;
        }
    }
    return -1;
}
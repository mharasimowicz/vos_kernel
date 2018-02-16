/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "../libc/string.h"
#include "../libc/stdio.h"
#include "../boot/multiboot.h"
#include "../drivers/cpu/cpu.h"
#include "../drivers/terminal/terminal.h"
#include "../drivers/io/io.h"
#include "../drivers/keyboard/keyboard.h"
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This system will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This system needs to be compiled with a ix86-elf compiler"
#endif


 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void show_basic_info(const int terminal);
void show_branding_info(const int terminal);
void show_multiboot_info(const multiboot_info_t* mbt, const int terminal);

int handleFirstInterrupt(struct regs * registers) {
    terminal_putchar('x');
}

void kernel_main(const multiboot_info_t* mbt, const uint32_t magic, const uint32_t esp)
{
    
	/* Initialize terminal interface */
	const uint32_t terminal = terminal_initialize();

    fputs("esp:", terminal);
    terminal_puthex(esp);
    terminal_newline();

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        fputs("Kernel did not boot from multiboot loader:\n", terminal);
        
        terminal_newline();
        fputs("Quitting system, as memory management probably should not work", terminal);
        return;
    }
         
    fputs("Hello, kernel World!\n", terminal);
    
    //show_basic_info(terminal);
    //show_branding_info(terminal);
    show_multiboot_info(mbt, terminal);
    
    /* Initialize core modules */
	gdt_install();      /* Global descriptor table */
    //idt.entries[0x21].
    //idt_set_gate(0x21, 0, 0, 0x6);
	//idt_install();      /* IDT */
	//isrs_install();     /* Interrupt service requests */
    //irq_install();      /* Hardware interrupt requests */
    
    //irq_install_handler(0x08, &handleFirstInterrupt);
    while(1) {}
    //
    // while(1) {
    //     const char scanCode = getScancode();

    //     terminal_puthex(scanCode);

    //     terminal_putchar(' ');
    // }
}



void show_basic_info(const int terminal) {
    char basicinfo[32];
    const uint8_t cpuid_support = cpu_has_cpuid_supported();
    if(cpuid_support > 0) {
        fputs("Processor supports CPUID!\n", terminal);
        fputs("Basic CPU info:\n", terminal);
        
        cpu_get_basic_info(basicinfo);

        fputs(basicinfo, terminal);
        fputs("\n", terminal);
    } else {
        fputs("Processor does not support CPUID!\n", terminal);
    }
}


void show_branding_info(const int terminal) {
    char brandinginfo[64];
    const uint8_t branding_support = cpu_has_brand_string_supported();
    
    if(branding_support > 0) {
        fputs("Processor supports branding!\n", terminal);
        fputs("Branding CPU info:\n", terminal);
        
        cpu_get_branding_info(brandinginfo);

        fputs(brandinginfo, terminal);
        fputs("\n", terminal);
    } else {
        fputs("Processor does not support branding!\n", terminal);
    }
}

void show_multiboot_info(const multiboot_info_t* mbt, const int terminal) {
    fputs("Booting from device #:", terminal);
    terminal_puthex(mbt->boot_device);
    terminal_newline();

    fputs("mem_lower:  ", terminal);
    terminal_puthex_l(mbt->mem_lower);
    terminal_newline();

    fputs("mem_upper:  ", terminal);
    terminal_puthex_l(mbt->mem_upper);
    terminal_newline();

    fputs("mmap_addr:  ", terminal);
    terminal_puthex_l(mbt->mmap_addr);
    terminal_newline();

    fputs("mmap_length:", terminal);
    terminal_puthex_l(mbt->mmap_length);
    terminal_newline();

    fputs("mods_count: ", terminal);
    terminal_puthex_l(mbt->mods_count);
    terminal_newline();

    fputs("mods_addr:  ", terminal);
    terminal_puthex_l(mbt->mods_addr);
    terminal_newline();

    fputs("memory_map:\n", terminal);

    memory_map_t* mmap = mbt->mmap_addr;
    int i = 0;

	while(mmap < mbt->mmap_addr + mbt->mmap_length) {
        terminal_putdec(i);
        fputs("|", terminal);
        terminal_puthex(mmap);
        terminal_newline();

		//...
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
        i++;        
	}
}
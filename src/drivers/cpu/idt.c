/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stdint.h>
#include "idt.h"
#include "../../libc/string.h"
#include "../io/io.h"

typedef struct {
	uint16_t offset_low;		// offset bits 0..15
	uint16_t selector;			// a code segment selector in GDT 
	uint8_t zero;				// unused, set to 0
	uint8_t type;				// type and attributes
	uint16_t offset_high;		// offset bits 16..31
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uintptr_t base;
} __attribute__((packed)) idt_pointer_t;

/* In the future we may need to put a lock on the access of this */
static struct {
	idt_entry_t entries[256];
	idt_pointer_t pointer;
} idt __attribute__((used));

#define ENTRY(X) (idt.entries[(X)])

void idt_set_gate(uint8_t num, idt_gate_t base, uint16_t selector, uint8_t typeflags) {
	ENTRY(num).offset_low = ((uintptr_t)base & 0xFFFF);
	ENTRY(num).offset_high = ((uintptr_t)base >> 16) & 0xFFFF;
	ENTRY(num).selector = selector;
	ENTRY(num).zero = 0;
	ENTRY(num).type = typeflags | 0x60;
}

void init_pics(int pic1, int pic2);

void idt_install(void) {
	idt_pointer_t * idtp = &idt.pointer;
	idtp->limit = sizeof idt.entries - 1;
	idtp->base = (uintptr_t)&ENTRY(0);
	memset(&ENTRY(0), 0, sizeof idt.entries);

	idt_flush((uintptr_t)idtp);
	init_pics(0x20, 0x28);
}

#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void init_pics(int pic1, int pic2)
{
   /* send ICW1 */
   outb(PIC1, ICW1);
   outb(PIC2, ICW1);

   /* send ICW2 */
   outb(PIC1 + 1, pic1);   
   outb(PIC2 + 1, pic2);   

   /* send ICW3 */
   outb(PIC1 + 1, 4);   
   outb(PIC2 + 1, 2);

   /* send ICW4 */
   outb(PIC1 + 1, ICW4);
   outb(PIC2 + 1, ICW4);

   /* disable all IRQs */
   outb(PIC1 + 1, 0xFF);
}

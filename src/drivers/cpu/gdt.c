/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stddef.h>
#include <stdint.h>
#include "gdt.h"
#include "tss.h"
#include "../../libc/string.h"

typedef struct {
	/* Limits */
	uint16_t limit_low;
	/* Segment address */
	uint16_t base_low;
	uint8_t base_middle;
	/* Access modes */
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_pointer_t;

/* In the future we may need to put a lock on the access of this */
static struct {
    gdt_entry_t entries[6];
    gdt_pointer_t pointer;
    tss_entry_t tss;
} gdt __attribute__((used));


#define ENTRY(X) (gdt.entries[(X)])

void gdt_set_gate(uint8_t num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran) {
	/* Base Address */
	ENTRY(num).base_low = (base & 0xFFFF);
	ENTRY(num).base_middle = (base >> 16) & 0xFF;
	ENTRY(num).base_high = (base >> 24) & 0xFF;
	/* Limits */
	ENTRY(num).limit_low = (limit & 0xFFFF);
	ENTRY(num).granularity = (limit >> 16) & 0X0F;
	/* Granularity */
	ENTRY(num).granularity |= (gran & 0xF0);
	/* Access flags */
	ENTRY(num).access = access;
}

static void write_tss(int32_t num, uint16_t ss0, uint32_t esp0);

//
// installing gdt - right now, we are are using all available RAM for all segments
//
void gdt_install(void) {
	gdt_pointer_t *gdtp = &gdt.pointer;
	gdtp->limit = sizeof gdt.entries - 1;
	gdtp->base = (uint32_t)&ENTRY(0);

	gdt_set_gate(0, 0, 0, 0, 0);                /* required NULL segment */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment */
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User code */
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User data */

	write_tss(5, 0x10, 0x0); /* Task segment */

	/* Go go go */
	gdt_flush((uint32_t)gdtp);
	tss_flush();
}

static void write_tss(int32_t num, uint16_t ss0, uint32_t esp0) {
	tss_entry_t * tss = &gdt.tss;
	uint32_t base = (uint32_t)tss;
	uint32_t limit = base + sizeof *tss;

	/* Add the TSS descriptor to the GDT */
	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(tss, 0x0, sizeof *tss);

	tss->ss0 = ss0;
	tss->esp0 = esp0;
	tss->cs = 0x0b;
	tss->ss = 0x13;
	tss->ds = 0x13;
	tss->es = 0x13;
	tss->fs = 0x13;
	tss->gs = 0x13;

	tss->iomap_base = sizeof *tss;
}

void set_kernel_stack(uint32_t stack) {
	/* Set the kernel stack */
	gdt.tss.esp0 = stack;
}
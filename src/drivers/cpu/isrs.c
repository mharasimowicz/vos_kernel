/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stddef.h>
#include <stdint.h>
#include "../../libc/stdio.h"
#include "../terminal/terminal.h"
#include "idt.h"
#include "isrs.h"

/* The count is treated as is when setting up IDT gates. However there is an
 * additional ISR for the system call vector which is handled explicitly since
 * it's mapped at a different address.
 */
#define ISR_COUNT 32
#define SYSCALL_VECTOR 0x7F

static struct {
	size_t index;
	void (*stub)(void);
} isrs[32 + 1] __attribute__((used));

static irq_handler_t isr_routines[256] = { 0 };

// extern char kernel_symbols_start[];
// extern char kernel_symbols_end[];



static kernel_symbol_t kernel_symbols[ISR_COUNT + 1];

/* Cannot use symboltable here because symbol_find is used during initialization
 * of IRQs and ISRs.
 */
// void (* symbol_find(const char * name))(void) {
// 	kernel_symbol_t * k = (kernel_symbol_t *)&kernel_symbols_start;

// 	while ((uintptr_t)k < (uintptr_t)&kernel_symbols_end) {
// 		if (strcmp(k->name, name)) {
// 			k = (kernel_symbol_t *)((uintptr_t)k + sizeof *k + strlen(k->name) + 1);
// 			continue;
// 		}
// 		return (void (*)(void))k->addr;
// 	}

// 	return NULL;
// }

void isrs_install_handler(size_t isrs, irq_handler_t handler) {
	isr_routines[isrs] = handler;
}

void isrs_uninstall_handler(size_t isrs) {
	isr_routines[isrs] = 0;
}

void isrs_install(void) {
	//char buffer[16];
	kernel_symbol_t* k = (kernel_symbol_t *)kernel_symbols;

	for (int i = 0; i < ISR_COUNT; i++) {
		// sprintf(buffer, "_isr%d", i);
		isrs[i].index = i;


		isrs[i].stub = ((uintptr_t)k + sizeof (kernel_symbol_t) * i);//symbol_find(buffer);
	}
	isrs[ISR_COUNT].index = SYSCALL_VECTOR;
	isrs[ISR_COUNT].stub = ((uintptr_t)k + sizeof (kernel_symbol_t) * ISR_COUNT);

	for (int i = 0; i < ISR_COUNT + 1; i++) {
		idt_set_gate(isrs[i].index, isrs[i].stub, 0x08, 0x8E);
	}
}

static const char *exception_messages[32] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void fault_handler(struct regs * r) {
	irq_handler_t handler = isr_routines[r->int_no];
	if (handler) {
		handler(r);
	} else {
        terminal_writestring("Unhandled exception:[");
        terminal_putdec(r->int_no);
        terminal_writestring("]\n");
        terminal_writestring(exception_messages[r->int_no]);
        terminal_newline();
        terminal_writestring("Hanging now..");
        while(1) {}
		// debug_print(CRITICAL, "Unhandled exception: [%d] %s", r->int_no, exception_messages[r->int_no]);
		// HALT_AND_CATCH_FIRE("Process caused an unhandled exception", r);
		// STOP;
	}
}
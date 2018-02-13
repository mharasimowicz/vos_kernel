/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _IRQ_H
#define _IRQ_H 1

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

typedef struct {
	uintptr_t addr;
	char name[];
} kernel_symbol_t;

typedef struct regs regs_t;

typedef void (*irq_handler_t) (struct regs *);
typedef int (*irq_handler_chain_t) (struct regs *);

void irq_install_handler(size_t irq, irq_handler_chain_t handler);
void irq_install(void);

#endif
/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _IDT_H
#define _IDT_H 1

typedef void (*idt_gate_t)(void);

extern void idt_install(void);

extern void idt_flush(uint32_t ptr);

extern void idt_set_gate(uint8_t num, idt_gate_t base, uint16_t selector, uint8_t flags);

#endif
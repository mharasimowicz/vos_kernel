/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _IDT_H
#define _IDT_H 1

extern void idt_install(void);

extern void idt_flush(uint32_t ptr);

#endif
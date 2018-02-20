/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _IDT_H
#define _IDT_H 1

extern void idt_install(void);

extern void idt_flush(uint32_t ptr);

extern void handle_interrupt(uint8_t interruptNumber, unsigned long handlerPtr);

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};


#endif
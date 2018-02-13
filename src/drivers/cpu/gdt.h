/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _GDT_H
#define _GDT_H 1

extern void gdt_install(void);

extern void gdt_flush(uint32_t ptr);

#endif
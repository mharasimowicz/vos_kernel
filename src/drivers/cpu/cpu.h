/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _CPU_H
#define _CPU_H 1

#include "gdt.h"
#include "idt.h"
#include "isrs.h"
#include "irq.h"

extern int cpu_has_cpuid_supported(void);

extern int cpu_has_brand_string_supported(void);

extern int cpu_get_basic_info(char* text);

extern int cpu_get_branding_info(char* text);

#endif
/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stddef.h>
#include <stdint.h>

#ifndef __STRING_H
#define __STRING_H

size_t strlen(const char* str);

void* memset(void * dest, int c, size_t n);

#endif
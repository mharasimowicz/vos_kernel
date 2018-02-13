/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

void* memset(void* dest, int c, size_t n) {
	asm volatile("cld; rep stosb"
	             : "=c"((int){0})
	             : "D"(dest), "a"(c), "c"(n)
	             : "flags", "memory");
	return dest;
}
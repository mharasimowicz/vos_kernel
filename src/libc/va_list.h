/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef __VALIST_H
#define __VALIST_H 1

typedef __builtin_va_list va_list;
#define va_start(ap,last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap,type) __builtin_va_arg(ap,type)
#define va_copy(dest, src) __builtin_va_copy(dest,src)


#endif
/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef __STDIO_H
#define __STDIO_H 1

#include "va_list.h"
// const int stdout = 0;
// const int stdin = 1;
// const int stderr = -1;

int fputs(const char* str, int stream); //FILE *stream);
int fputc(const char c, int stream); //FILE *stream);
int putc(const char c, int stream); // FILE *stream);
int putchar(const char c);
int puts(const char *s);
int sprintf ( char * str, const char * format, ...); //...
void printf (const char *format, ...);

#endif
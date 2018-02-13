/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stddef.h>
#include "../io/io.h"

char getScancode()
{
    char flag = inb(0x64);
    while(!(flag & 1)) {
        flag = inb(0x64);
    }
    return inb(0x60);
}
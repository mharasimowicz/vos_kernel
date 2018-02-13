/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#ifndef _ISRS_H
#define _ISRS_H 1

#include "irq.h"

extern void isrs_install(void);
extern void isrs_install_handler(size_t isrs, irq_handler_t);
extern void isrs_uninstall_handler(size_t isrs);

#endif
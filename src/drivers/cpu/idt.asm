;
; VOS kernel
; Copyright 2018 Michal Harasimowicz
;
[bits 32]
SECTION .text

global idt_flush
global keyboard_handler

extern keyboard_handler_main

idt_flush:
    mov edx, [esp + 4]  ;reading idt pointer from stack
	lidt [edx]
	sti 				;turn on interrupts
	ret

keyboard_handler:                 
	call    keyboard_handler_main
	iretd
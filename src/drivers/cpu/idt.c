/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#include <stdint.h>
#include "idt.h"
#include "../io/io.h"

#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define IDT_SIZE 256


struct IDT_entry IDT[IDT_SIZE];

void handle_interrupt(uint8_t interruptNumber, unsigned long handlerPtr)
{
	unsigned long interrupt_address;

	/* populate IDT entry of interrupt */
	interrupt_address = handlerPtr;
	IDT[interruptNumber].offset_lowerbits = interrupt_address & 0xffff;
	IDT[interruptNumber].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[interruptNumber].zero = 0;
	IDT[interruptNumber].type_attr = INTERRUPT_GATE;
	IDT[interruptNumber].offset_higherbits = (interrupt_address & 0xffff0000) >> 16;
}

void idt_install(void) {
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	outb(0x21 , 0x00);
	outb(0xA1 , 0x00);

	/* ICW4 - environment info */
	outb(0x21 , 0x01);
	outb(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	idt_flush(idt_ptr);
}


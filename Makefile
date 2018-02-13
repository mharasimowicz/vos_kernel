CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
AC = nasm
ACC = i686-elf-as # gcc assembly compiler probably will be removed later
AFLAGS = -f elf
RUNTIME = qemu-system-i386
ISOFILE = vos.iso
BINFILE = vos.bin


runcd: makeiso
	$(RUNTIME) -cdrom bin/$(ISOFILE)


makeiso: bin_file
	mkdir -p isodir/boot/grub
	cp bin/$(BINFILE) isodir/boot/$(BINFILE)
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o bin/$(ISOFILE) isodir


run: bin_file
	$(RUNTIME) -kernel bin/$(BINFILE)

all: bin_file


bin_folder:
	mkdir bin

bin_file: boot.o kernel.o cpu.o io.o gdt_asm.o gdt.o idt_asm.o idt.o tss.o isrs.o irq.o bin_folder
	$(CC) -T src/linker.ld -o bin/$(BINFILE) -ffreestanding -O2 -nostdlib bin/boot.o bin/kernel.o bin/cpu.o bin/io.o bin/string.o bin/stdio.o bin/terminal.o bin/keyboard.o bin/gdt_asm.o bin/gdt.o bin/idt_asm.o bin/idt.o bin/tss.o bin/isrs.o bin/irq.o -lgcc

cpu.o: bin_folder
	$(AC) $(AFLAGS) -o bin/cpu.o src/drivers/cpu/cpu.asm

gdt.o: bin_folder
	$(CC) -c src/drivers/cpu/gdt.c -o bin/gdt.o $(CFLAGS)

gdt_asm.o: bin_folder
	$(AC) $(AFLAGS) -o bin/gdt_asm.o src/drivers/cpu/gdt.asm

tss.o: bin_folder
	$(AC) $(AFLAGS) -o bin/tss.o src/drivers/cpu/tss.asm


idt.o: bin_folder
	$(CC) -c src/drivers/cpu/idt.c -o bin/idt.o $(CFLAGS)

idt_asm.o: bin_folder
	$(AC) $(AFLAGS) -o bin/idt_asm.o src/drivers/cpu/idt.asm

isrs.o: bin_folder
	$(CC) -c src/drivers/cpu/isrs.c -o bin/isrs.o $(CFLAGS)

irq.o: bin_folder
	$(CC) -c src/drivers/cpu/irq.c -o bin/irq.o $(CFLAGS)


io.o: bin_folder
	$(CC) -c src/drivers/io/io.c -o bin/io.o $(CFLAGS)

boot.o: bin_folder
	$(ACC) src/boot/boot.asm -o bin/boot.o

kernel.o: bin_folder string.o stdio.o terminal.o keyboard.o gdt.o gdt_asm.o idt.o idt_asm.o tss.o isrs.o
	$(CC) -c src/kernel/kernel.c -o bin/kernel.o $(CFLAGS)

stdio.o: bin_folder
	$(CC) -c src/libc/stdio/stdio.c -o bin/stdio.o $(CFLAGS)

string.o: bin_folder
	$(CC) -c src/libc/string/string.c -o bin/string.o $(CFLAGS)

terminal.o: bin_folder
	$(CC) -c src/drivers/terminal/terminal.c -o bin/terminal.o $(CFLAGS)

keyboard.o: bin_folder
	$(CC) -c src/drivers/keyboard/keyboard.c -o bin/keyboard.o $(CFLAGS)

.PHONY: clean

clean:
	rm bin -rf
	rm isodir -rf
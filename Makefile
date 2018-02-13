runcd: makeiso
	qemu-system-i386 -cdrom bin/vos.iso


makeiso: vos.bin
	mkdir -p isodir/boot/grub
	cp bin/vos.bin isodir/boot/vos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o bin/vos.iso isodir


run: vos.bin
	qemu-system-i386 -kernel bin/vos.bin

all: vos.bin


clean:
	rm bin -rf
	rm isodir -rf

bin_folder:
	mkdir bin

vos.bin: boot.o kernel.o cpu.o io.o gdt_asm.o gdt.o idt_asm.o idt.o tss.o isrs.o irq.o bin_folder
	i686-elf-gcc -T src/linker.ld -o bin/vos.bin -ffreestanding -O2 -nostdlib bin/boot.o bin/kernel.o bin/cpu.o bin/io.o bin/string.o bin/stdio.o bin/terminal.o bin/keyboard.o bin/gdt_asm.o bin/gdt.o bin/idt_asm.o bin/idt.o bin/tss.o bin/isrs.o bin/irq.o -lgcc

cpu.o: bin_folder
	nasm -f elf -o bin/cpu.o src/drivers/cpu/cpu.asm

gdt.o: bin_folder
	i686-elf-gcc -c src/drivers/cpu/gdt.c -o bin/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gdt_asm.o: bin_folder
	nasm -f elf -o bin/gdt_asm.o src/drivers/cpu/gdt.asm

tss.o: bin_folder
	nasm -f elf -o bin/tss.o src/drivers/cpu/tss.asm


idt.o: bin_folder
	i686-elf-gcc -c src/drivers/cpu/idt.c -o bin/idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

idt_asm.o: bin_folder
	nasm -f elf -o bin/idt_asm.o src/drivers/cpu/idt.asm

isrs.o: bin_folder
	i686-elf-gcc -c src/drivers/cpu/isrs.c -o bin/isrs.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

irq.o: bin_folder
	i686-elf-gcc -c src/drivers/cpu/irq.c -o bin/irq.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra


io.o: bin_folder
	i686-elf-gcc -c src/drivers/io/io.c -o bin/io.o  -std=gnu99 -ffreestanding -O2 -Wall -Wextra 

boot.o: bin_folder
	i686-elf-as src/boot/boot.asm -o bin/boot.o

kernel.o: bin_folder string.o stdio.o terminal.o keyboard.o gdt.o gdt_asm.o idt.o idt_asm.o tss.o isrs.o
	i686-elf-gcc -c src/kernel/kernel.c -o bin/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

stdio.o: bin_folder
	i686-elf-gcc -c src/libc/stdio/stdio.c -o bin/stdio.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

string.o: bin_folder
	i686-elf-gcc -c src/libc/string/string.c -o bin/string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

terminal.o: bin_folder
	i686-elf-gcc -c src/drivers/terminal/terminal.c -o bin/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

keyboard.o: bin_folder
	i686-elf-gcc -c src/drivers/keyboard/keyboard.c -o bin/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
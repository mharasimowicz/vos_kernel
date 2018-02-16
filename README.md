# VOS - experimental system

## kernel module

Currently supports only x86 architecture.

Copyright 2018 Michał Harasimowicz

## requirements

### for building
- gcc cross compiler
- grub-mkrescue
- nasm
- xorriso

### for running
- qemu

## build & run

```bash
make runcd
```

## debugging

```bash
make debug

# connect from second terminal

gdb
(gdb) target remote localhost:1234
(gdb) symbol_file ./bin/vos.bin
(gdb) break kernel_main
(gdb) continue
```
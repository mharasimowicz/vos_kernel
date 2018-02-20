/*
	VOS kernel
	Copyright 2018 Michal Harasimowicz
*/

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "../../libc/string.h"

/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
 
uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
uint32_t terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
    return 0;
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c) {
    if(c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {   
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_row = 0;
            }
        }
    }
}
 
void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}

void terminal_puthex(const int number) {
    uint32_t mask = 0xF0000000;
    const char ascii = 0x30;
    
    int i = 7;
    
    for(i = 7; i >= 0; i--) {
        
        uint32_t sign = number & mask;
        
        uint32_t shifted = sign >> (i * 4);
        
        char to_show = ((char)shifted) + ascii;
        
        if(shifted > 0x9) {
            to_show += 7;
        }
        
        terminal_putchar(to_show);
        
        mask = mask >> 4;
    }
}

void terminal_putdec(const uint32_t number) {
    uint32_t temp = number;
    uint32_t mask = 1000000000;
    const char ascii = 0x30;
    
    uint32_t i = 10;
    uint32_t m = 10;
    
    for(; i > 0; i--) {
        
        uint32_t shifted = temp / mask;
        
        char to_show = ((char)shifted) + ascii;
        
        terminal_putchar(to_show);
        
        temp = temp - (shifted * mask);
        
        mask = mask / m;
    }
}

void terminal_putdec_l(const uint64_t number) {
    uint64_t temp = number;
    uint64_t mask = 1000000000000000000;
    const char ascii = 0x30;
    
    uint64_t i = 20;
    uint64_t m = 10;
    
    for(; i > 0; i--) {
        
        uint64_t shifted = temp / mask;
        
        char to_show = ((char)shifted) + ascii;
        
        terminal_putchar(to_show);
        
        temp = temp - (shifted * mask);
        
        mask = mask / m;
    }
}

void terminal_puthex_l(const long number) {
    uint64_t mask = 0xF000000000000000;
    const char ascii = 0x30;
    
    int i = 15;
    
    for(i = 15; i >= 0; i--) {
        
        uint64_t sign = number & mask;
        
        uint64_t shifted = sign >> (i * 4);
        
        
        
        char to_show = ((char)shifted) + ascii;
        
        if(shifted > 0x9) {
            to_show += 7;
        }
        
        terminal_putchar(to_show);
        
        mask = mask >> 4;
    }
}

void terminal_newline() {
	terminal_column = 0;
    terminal_row++;
}
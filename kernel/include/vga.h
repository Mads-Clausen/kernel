#ifndef __VGA_H_INCLUDED__
#define __VGA_H_INCLUDED__

#include <stdint.h>
#include <system.h>

enum VGA_COLOR
{
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

#define VGA_TEXT_WIDTH 80
#define VGA_TEXT_HEIGHT 25

enum VGA_MODE
{
    VGA_TEXTMODE
};

struct vga_entry
{
    uint8_t blink;
    enum VGA_COLOR bg_color, fg_color;
    char c;
};

/** Initialise the VGA driver */
void vga_init();

/** Set the VGA mode */
void vga_setmode(enum VGA_MODE mode);

/** Generate a VGA entry with attributes(for text mode) */
uint16_t _vga_make_entry(char c, enum VGA_COLOR _fg_color, enum VGA_COLOR _bg_color, uint8_t blink);

/** Generate a VGA entry with attributes(for text mode) */
uint16_t vga_make_entry(struct vga_entry *e);

/** Set the cursor position(for text mode) */
void vga_set_cursorpos(int x, int y);

/** Put an entry in the graphics memory */
void vga_putentry(uint16_t entry, int x, int y);

/** Scroll down n lines */
void vga_scroll_down(int n);

/** Print a character to the VGA memory */
void putch(char c);

/** Print a character to the VGA memory, with color */
void putch_c(char c, enum VGA_COLOR fg, enum VGA_COLOR bg);

/** Print a string to the VGA memory */
void puts(char *str);

/** Print a string to the VGA memory, with color */
void puts_c(char *str, enum VGA_COLOR fg, enum VGA_COLOR bg);

#endif
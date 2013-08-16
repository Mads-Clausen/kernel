#include <vga.h>

enum VGA_MODE current_mode = VGA_TEXTMODE;

uint16_t *buffer;
enum VGA_COLOR vga_fg = COLOR_DEFAULT_FG, vga_bg = COLOR_DEFAULT_BG;
int cur_x, cur_y;

void vga_init()
{
    buffer = (uint16_t *) 0xB8000;
    unsigned int x, y;
    for(x = 0; x < VGA_TEXT_WIDTH; ++x)
        for(y = 0; y < VGA_TEXT_HEIGHT; ++y)
            buffer[x + y * VGA_TEXT_WIDTH] = _vga_make_entry(' ', vga_fg, vga_bg, 0);
}

void vga_setmode(enum VGA_MODE mode)
{
    current_mode = mode;
}

uint16_t _vga_make_entry(char c, enum VGA_COLOR _fg_color, enum VGA_COLOR _bg_color, uint8_t blink)
{
    /* http://www.anickyan.kd.io/kernel/docs/vga_entry.png */

    if(current_mode == VGA_TEXTMODE)
    {
        uint8_t fg_color = (uint8_t) _fg_color,
                bg_color = (uint8_t) _bg_color;

        uint16_t entry;
        entry = 0 | ((blink == 0 ? 0 : 1) << 16);
        uint16_t color = (fg_color | (bg_color << 4)) << 8;
        entry |= color;
        entry |= c;

        return entry;
    }

    return 0;
}

/* easy interface for _vga_make_entry(), this time using a struct */
uint16_t vga_make_entry(struct vga_entry *e)
{
    return _vga_make_entry(e->c, e->fg_color, e->bg_color, e->blink);
}

void vga_scroll_down(int n)
{
    cur_y -= n;

    /* move lines up */
    unsigned int y, x;
    for(y = n; y < VGA_TEXT_HEIGHT; ++y)
        for(x = 0; x < VGA_TEXT_WIDTH; ++x)
            buffer[(y - 1) * VGA_TEXT_WIDTH + x] = 0 | buffer[y * VGA_TEXT_WIDTH + x];

    /* clear the bottom n lines */
    for(y = VGA_TEXT_HEIGHT - 1; y >= VGA_TEXT_HEIGHT - n; --y)
        for(x = 0; x < VGA_TEXT_WIDTH; ++x)
            buffer[y * VGA_TEXT_WIDTH + x] = 0;
}

void vga_set_cursorpos(int x, int y)
{
    if(current_mode == VGA_TEXTMODE)
    {
        cur_x = x;
        cur_y = y;

        if(cur_x >= VGA_TEXT_WIDTH)
        {
            cur_x = 0;
            if(++cur_y >= VGA_TEXT_HEIGHT)
            {
                vga_scroll_down(1);
            }
        }

        if(cur_y >= VGA_TEXT_HEIGHT)
        {
            vga_scroll_down(1);
        }

        uint16_t pos = y * VGA_TEXT_WIDTH + x;
        outb(0x3D4, 0x0F);
        outb(0x3D5, (uint8_t) (pos & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    }
}

void vga_putentry(uint16_t entry, int x, int y)
{
    if(current_mode == VGA_TEXTMODE)
    {
        buffer[y * VGA_TEXT_WIDTH + x] = entry;
    }
}

void putch(char c)
{
    if(current_mode == VGA_TEXTMODE)
    {
        if(c == '\n')
        {
            vga_set_cursorpos(0, cur_y + 1);
            return;
        }

        uint16_t entry = _vga_make_entry(c, vga_fg, vga_bg, 0);
        vga_putentry(entry, cur_x, cur_y);
        vga_set_cursorpos(cur_x + 1, cur_y);
    }
}

void putch_c(char c, enum VGA_COLOR fg, enum VGA_COLOR bg)
{
    if(current_mode == VGA_TEXTMODE)
    {
        if(c == '\n')
        {
            vga_set_cursorpos(0, cur_y + 1);
            return;
        }

        uint16_t entry = _vga_make_entry(c, fg, bg, 0);
        vga_putentry(entry, cur_x, cur_y);
        vga_set_cursorpos(cur_x + 1, cur_y);
    }
}


void puts(char *str)
{
    if(current_mode == VGA_TEXTMODE)
    {
        for(; *str; str++)
        {
            if(*str == '\n')
            {
                vga_set_cursorpos(0, cur_y + 1);
                continue;
            }

            putch(*str);
        }
    }
}

void puts_c(char *str, enum VGA_COLOR fg, enum VGA_COLOR bg)
{
    if(current_mode == VGA_TEXTMODE)
    {
        for(; *str; str++)
        {
            if(*str == '\n')
            {
                vga_set_cursorpos(0, cur_y + 1);
                continue;
            }

            putch_c(*str, fg, bg);
        }
    }
}

#include <types.h>
#include <globals.h>
#include <terminal.h>

#define CHAR_W 6
#define CHAR_H 9
#define GLYPH_W 5
#define GLYPH_H 7

static size_t cursor_x;
static size_t cursor_y;

static const u8 font[26][7] = {
    {14, 17, 17, 31, 17, 17, 17}, {30, 17, 17, 30, 17, 17, 30}, {14, 17, 16, 16, 16, 17, 14},
    {30, 17, 17, 17, 17, 17, 30}, {31, 16, 16, 30, 16, 16, 31}, {31, 16, 16, 30, 16, 16, 16},
    {14, 17, 16, 23, 17, 17, 14}, {17, 17, 17, 31, 17, 17, 17}, {14, 4, 4, 4, 4, 4, 14},
    {7, 2, 2, 2, 18, 18, 12}, {17, 18, 20, 24, 20, 18, 17}, {16, 16, 16, 16, 16, 16, 31},
    {17, 27, 21, 21, 17, 17, 17}, {17, 25, 21, 19, 17, 17, 17}, {14, 17, 17, 17, 17, 17, 14},
    {30, 17, 17, 30, 16, 16, 16}, {14, 17, 17, 17, 21, 18, 13}, {30, 17, 17, 30, 20, 18, 17},
    {15, 16, 16, 14, 1, 1, 30}, {31, 4, 4, 4, 4, 4, 4}, {17, 17, 17, 17, 17, 17, 14},
    {17, 17, 17, 21, 21, 21, 10}, {17, 17, 17, 21, 21, 21, 10}, {17, 17, 10, 4, 10, 17, 17},
    {17, 17, 10, 4, 4, 4, 4}, {31, 1, 2, 4, 8, 16, 31}
};

static const pixel_t FG = 0x00E6E6E6;
static const pixel_t BG = 0x00101820;

static inline pixel_t *row_ptr(u32 y) {
    return (pixel_t *)((u8 *)fb_addr + ((size_t)y * fb_pixperline));
}

static u8 glyph_row(char c, unsigned row) {
    if (c >= 'a' && c <= 'z')
        c = (char)(c - 'a' + 'A');

    if (c >= 'A' && c <= 'Z' && row < GLYPH_H)
        return font[(unsigned)(c - 'A')][row];

    return 0;
}

static void clear_row(u32 y) {
    pixel_t *row = row_ptr(y);

    for (u32 x = 0; x < fb_w; ++x)
        row[x] = BG;
}

static void clear_screen(void) {
    for (u32 y = 0; y < fb_h; ++y)
        clear_row(y);

    cursor_x = 0;
    cursor_y = 0;
}

static void scroll(void) {
    if (fb_h <= CHAR_H)
        return;

    for (u32 y = 0; y + CHAR_H < fb_h; ++y) {
        pixel_t *dst = row_ptr(y);
        pixel_t *src = row_ptr(y + CHAR_H);

        for (u32 x = 0; x < fb_w; ++x)
            dst[x] = src[x];
    }

    for (u32 y = fb_h - CHAR_H; y < fb_h; ++y)
        clear_row(y);
}

void terminal_init(void) {
    if (!fb_addr || !fb_w || !fb_h || !fb_pixperline)
        return;

    clear_screen();
}

void terminal_putchar(char c) {
    if (!fb_addr || !fb_w || !fb_h || !fb_pixperline)
        return;

    if (c == '\r') {
        cursor_x = 0;
        return;
    }

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += CHAR_H;

        if (cursor_y + GLYPH_H > fb_h) {
            scroll();
            cursor_y = fb_h - CHAR_H;
        }

        return;
    }

    if (cursor_x + GLYPH_W > fb_w) {
        cursor_x = 0;
        cursor_y += CHAR_H;
    }

    if (cursor_y + GLYPH_H > fb_h) {
        scroll();
        cursor_y = fb_h - CHAR_H;
    }

    for (unsigned gy = 0; gy < GLYPH_H; ++gy) {
        u8 bits = glyph_row(c, gy);
        pixel_t *row = row_ptr((u32)(cursor_y + gy));

        for (unsigned gx = 0; gx < GLYPH_W; ++gx) {
            row[cursor_x + gx] =
                (bits & (1u << (GLYPH_W - 1 - gx))) ? FG : BG;
        }
    }

    cursor_x += CHAR_W;
}

void terminal_write(const char *str) {
    if (!str)
        return;

    while (*str)
        terminal_putchar(*str++);
}

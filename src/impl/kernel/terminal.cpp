#include <terminal.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <io.h>

#define VGA_BUFFER_ADDR 0xb8000
#define VGA_COLS 80
#define VGA_ROWS 25

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);


void terminalClearRow(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < VGA_COLS; col++) {
        buffer[col + VGA_COLS * row] = empty;
    }
}

void terminalClear() {
    for (int y = 0; y < VGA_ROWS; y++) {
        for (int x = 0; x < VGA_COLS; x++) {
            terminalSetColor(color, PRINT_COLOR_BLACK);
            buffer[(y * VGA_COLS + x)] = (struct Char) {
                character: ' ',
                color: color,
            };
        }
    }

    col = 0;
    row = 0;
}

void printNewline() {
    col = 0;

    if (row < VGA_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < VGA_ROWS; row++) {
        for (size_t col = 0; col < VGA_COLS; col++) {
            struct Char character = buffer[col + VGA_COLS * row];
            buffer[col + VGA_COLS * (row - 1)] = character;
        }
    }

    terminalClearRow(VGA_COLS - 1);
}

void printChar(char character) {
    if (character == '\n') {
        printNewline();
        return;
    }

    if (col > VGA_COLS) {
        printNewline();
    }

    buffer[col + VGA_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color
    };

    col++;
}

void printStr(char* str) {
    for (size_t i = 0; 1; i++) {
        const int character = str[i];

        if (character == '\0') {
            return;
        }

        printChar(character);
    }
}

void terminalSetColor(uint8_t foreground, uint8_t background) {
    color = foreground | (background << 4);
}

void terminalBackspace() {
    if (col == 0) {
        return;
    }

    col--;
    printChar(' ');
    col--;
    terminalSetCursorPos(col, row);
}

void print(char* str) {
    printStr(str);
    printNewline();
}

void terminalSetCursorPos(size_t x, size_t y) {
    uint16_t pos = y * VGA_COLS + x;

    outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));

    col = x;
    row = y;
}

CursorPosition terminalGetCursorPos() {
    return {
        x: col, y: row
    };
}

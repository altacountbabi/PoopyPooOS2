#include <terminal.h>
#include <stdio.h>
#include <stdint.h>
#include <serial.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>

char* intToString(int num, char* buffer) {
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    const int maxLength = 12;
    char tempBuffer[maxLength];

    int i = 0;
    do {
        tempBuffer[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    if (isNegative) {
        tempBuffer[i++] = '-';
    }

    int length = i;
    int j = 0;
    while (j < length) {
        buffer[j] = tempBuffer[length - j - 1];
        ++j;
    }
    buffer[j] = '\0';

    return buffer;
}

void keyboardInterruptHandler() {
    char scancode = keyboard::readChar();

    char character = keyboard::scancodeToASCII(scancode);

    switch (character) {
    case '\b':
        terminalBackspace();
        break;
    case 0:
        break;
    default:
        printf("%c", character);
        CursorPosition position = terminalGetCursorPos();
        terminalSetCursorPos(position.x, position.y);
        goto scancodeSwitch;
        break;
    }

    scancodeSwitch:
        switch (scancode) {
        case 0x4B: // left arrow
            terminalArrowKey(0);
            break;
        case 0x4D: // right arrow
            terminalArrowKey(1);
            break;
        }
}

extern "C" void kernel_main() {
    terminalClear();
    terminalSetCursorPos(0, 0);
    terminalSetCursorState(true);
    terminalSetColor(PRINT_COLOR_LIGHT_GRAY, PRINT_COLOR_BLACK);

    print("Loading keyboard driver... ");
    keyboard::init();
    print("All drivers were loaded correctly.");
    while (1) {
        keyboardInterruptHandler();
    }
    
    while (1) {
        asm volatile ("hlt");
    }
}
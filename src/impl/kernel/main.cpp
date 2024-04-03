#include <terminal.h>
#include <stdio.h>
#include <stdint.h>
#include <serial.h>

// Define keyboard I/O ports (example values)
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Define keyboard commands
#define CMD_ENABLE_KEYBOARD 0xAE
#define CMD_DISABLE_KEYBOARD 0xAD
#define CMD_READ_SCANCODE 0x20

// Keyboard driver class
class KeyboardDriver {
public:
    KeyboardDriver() {}
    void init() {
        sendCommand(CMD_ENABLE_KEYBOARD);
    }

    char readChar() {
        while (!dataAvailable()) {}
        return readData();
    }

private:
    bool dataAvailable() {
        return (inb(KEYBOARD_STATUS_PORT) & 0x01);
    }

    char readData() {
        return inb(KEYBOARD_DATA_PORT);
    }

    void sendCommand(uint8_t cmd) {
        while (inb(KEYBOARD_STATUS_PORT) & 0x02) {}
        outb(KEYBOARD_STATUS_PORT, cmd);
    }

    uint8_t inb(uint16_t port) {
        uint8_t data;
        asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

    void outb(uint16_t port, uint8_t data) {
        asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
    }
};

char scancodeToASCII(char scancode) {
    char kbd_US [840] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
      '\t', /* <-- Tab */
      'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
        0, /* <-- control key */
      'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
      '*',
        0,  /* Alt */
      ' ',  /* Space bar */
        0,  /* Caps lock */
        0,  /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  /* < ... F10 */
        0,  /* 69 - Num lock*/
        0,  /* Scroll Lock */
        0,  /* Home key */
        0,  /* Up Arrow */
        0,  /* Page Up */
      '-',
        0,  /* Left Arrow */
        0,
        0,  /* Right Arrow */
      '+',
        0,  /* 79 - End key*/
        0,  /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        0,  /* Delete Key */
        0,   0,   0,
        0,  /* F11 Key */
        0,  /* F12 Key */
        0,  /* All other keys are undefined */
    };

    if (kbd_US[scancode] != 0) {
        return kbd_US[scancode];
    }

    return '\0';
}

KeyboardDriver keyboard;


// Function to convert an integer to a string
char* intToString(int num, char* buffer) {
    // Check if the number is negative
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    // Define a maximum length for the buffer
    const int maxLength = 12; // Enough to hold the maximum value of a 32-bit integer in base 10, including the sign and null terminator
    char tempBuffer[maxLength]; // Temporary buffer for generating the string in reverse order

    // Generate digits in reverse order
    int i = 0;
    do {
        tempBuffer[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add negative sign if necessary
    if (isNegative) {
        tempBuffer[i++] = '-';
    }

    // Reverse the string
    int length = i;
    int j = 0;
    while (j < length) {
        buffer[j] = tempBuffer[length - j - 1];
        ++j;
    }
    buffer[j] = '\0'; // Add null terminator

    return buffer;
}

void keyboardInterruptHandler() {
    char scancode = keyboard.readChar();
    char character = scancodeToASCII(scancode);

    if (character == 0) return;

    if (character == '\b') {
        terminalBackspace();
        return;
    }

    printf("%c", character);
    CursorPosition position = terminalGetCursorPos();
    terminalSetCursorPos(position.x, position.y);
}

extern "C" void kernel_main() {
    terminalClear();
    terminalSetColor(PRINT_COLOR_LIGHT_GRAY, PRINT_COLOR_BLACK);
    terminalSetCursorPos(0, 0);

    keyboard.init();
    while (1) {
        keyboardInterruptHandler();
    }
    
    while (1) {
        asm volatile ("hlt");
    }
}
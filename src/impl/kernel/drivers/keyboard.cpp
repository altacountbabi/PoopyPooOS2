#include <drivers/keyboard.h>
#include <stdint.h>
#include <io.h>

namespace keyboard {
    void init() {
        sendCommand(CMD_ENABLE_KEYBOARD);
    }

    char readChar() {
        while (!dataAvailable()) {}
        return readData();
    }

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
}
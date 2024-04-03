#include <stdint.h>
#include <io.h>

void write_serial(char c) {
    // Wait until the serial port is ready to accept a byte
    while ((inb(0x3F8 + 5) & 0x20) == 0);
    // Write the character to the serial port
    outb(0x3F8, c);
}

// Function to write a null-terminated string to the serial port
void write_serial_string(const char* str) {
    // Iterate through each character in the string
    while (*str != '\0') {
        // Write the character to the serial port
        write_serial(*str);
        // Move to the next character in the string
        ++str;
    }
}
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define CMD_ENABLE_KEYBOARD 0xAE
#define CMD_DISABLE_KEYBOARD 0xAD
#define CMD_READ_SCANCODE 0x20

#include <stdint.h>
#include <io.h>

namespace keyboard {
    void init();
    char readChar();
    bool dataAvailable();
    char readData();
    void sendCommand(uint8_t cmd);
    char scancodeToASCII(char scancode);
}
#include <io.h>

#define VIDEO_MEMORY 0xA0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define VGA_MISC_WRITE  0x3C2
#define VGA_SEQ_INDEX   0x3C4
#define VGA_SEQ_DATA    0x3C5
#define VGA_CRTC_INDEX  0x3D4
#define VGA_CRTC_DATA   0x3D5
#define VGA_GC_INDEX    0x3CE
#define VGA_GC_DATA     0x3CF
#define VGA_AC_INDEX    0x3C0
#define VGA_AC_WRITE    0x3C0
#define VGA_AC_READ     0x3C1
#define VGA_INSTAT_READ 0x3DA


// TODO: Send a letter to god and ask him why this doesn't work
namespace vga {
    // Function to set VGA video mode
    void clearScreen() {
        char* screen = (char*)0xA0000;
        for (int i = 0; i < 320 * 200; ++i) {
            screen[i] = 0; // Assuming black is represented by 0
        }
    }

    void setVideoMode() {
        outb(VGA_MISC_WRITE, 0x63);

        // Unlock CRT registers
        outb(VGA_CRTC_INDEX, 0x03);
        outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
        outb(VGA_CRTC_INDEX, 0x11);
        outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);

        // Write to VGA sequencer registers
        outb(VGA_SEQ_INDEX, 0x00);
        outb(VGA_SEQ_DATA, 0x01);
        outb(VGA_SEQ_INDEX, 0x01);
        outb(VGA_SEQ_DATA, 0x01);
        outb(VGA_SEQ_INDEX, 0x02);
        outb(VGA_SEQ_DATA, 0x0F);
        outb(VGA_SEQ_INDEX, 0x03);
        outb(VGA_SEQ_DATA, 0x00);
        outb(VGA_SEQ_INDEX, 0x04);
        outb(VGA_SEQ_DATA, 0x0E);

        // Write to VGA graphics controller registers
        outb(VGA_GC_INDEX, 0x00);
        outb(VGA_GC_DATA, 0x05);
        outb(VGA_GC_INDEX, 0x01);
        outb(VGA_GC_DATA, 0x00);
        outb(VGA_GC_INDEX, 0x02);
        outb(VGA_GC_DATA, 0x0F);
        outb(VGA_GC_INDEX, 0x03);
        outb(VGA_GC_DATA, 0x00);
        outb(VGA_GC_INDEX, 0x04);
        outb(VGA_GC_DATA, 0x0E);
        outb(VGA_GC_INDEX, 0x05);
        outb(VGA_GC_DATA, 0x08);

        // Write to VGA attribute controller registers
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, 0x00);
        outb(VGA_AC_WRITE, 0x01);
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, 0x01);
        outb(VGA_AC_WRITE, 0x00);

        // Clear screen
        vga::clearScreen();
    }

    // Function to draw a pixel at position (x, y) with color
    void drawPixel(int x, int y, char color) {
        char* screen = (char*)VIDEO_MEMORY;
        int offset = y * SCREEN_WIDTH + x;
        screen[offset] = color;
    }
}
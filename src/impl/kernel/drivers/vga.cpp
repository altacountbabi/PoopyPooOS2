#define VIDEO_MEMORY 0xA0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

// TODO: set the video mode to actually draw to the screen
void drawPixel(int x, int y, char color) {
    char* screen = (char*)VIDEO_MEMORY;
    int offset = y * SCREEN_WIDTH + x;
    screen[offset] = color;
}
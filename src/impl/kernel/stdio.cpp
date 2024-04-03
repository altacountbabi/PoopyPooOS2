#include <terminal.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

void reverse(char str[]) {
    size_t length = strlen(str);
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0)  // Record sign
        n = -n;          // Make n positive
    i = 0;
    do {                 // Generate digits in reverse order
        str[i++] = n % 10 + '0';  // Get next digit
    } while ((n /= 10) > 0);      // Delete it
    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            switch (*(format + 1)) {
                case 'd': {
                    int num = va_arg(args, int);
                    // Convert integer to string
                    char str[32];  // Assume enough buffer space
                    int_to_ascii(num, str);
                    printStr(str);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char *);
                    printStr(str);
                    break;
                }
                case 'c': {
                    char ch = (char)va_arg(args, int);
                    printChar(ch);
                    break;
                }
                default:
                    printChar('%');
                    printChar(*(format + 1));
                    break;
            }
            format += 2;
        } else {
            printChar(*format++);
        }
    }

    va_end(args);
}
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Check if a character is a digit (0-9)
#define is_digit(c) ((c) >= '0' && (c) <= '9')

// Skip leading digits in a string and convert them to an integer
static int skip_atoi(const char **s) {
    int value = 0;
    while (is_digit(**s)) {
        value = value * 10 + (**s - '0');
        (*s)++;
    }
    return value;
}

// Flags for formatting
#define FLAG_ZEROPAD   (1 << 0)  // Pad with zeros
#define FLAG_LEFT      (1 << 1)  // Left-justify
#define FLAG_PLUS      (1 << 2)  // Show plus sign
#define FLAG_SPACE     (1 << 3)  // Show space for positive numbers
#define FLAG_SPECIAL   (1 << 4)  // Add '0x' for hex or '0' for octal
#define FLAG_SMALL     (1 << 5)  // Use lowercase for hex digits

// Convert a number to a string representation
static char *number_to_string(char *str, int num, int base, int width, int precision, int flags) {
    static const char *digits_upper = "0123456789ABCDEF";
    static const char *digits_lower = "0123456789abcdef";
    const char *digits = (flags & FLAG_SMALL) ? digits_lower : digits_upper;
    char buffer[32];  // Temporary buffer for storing digits
    int index = 0;
    bool is_negative = false;

    // Handle negative numbers
    if (flags & FLAG_PLUS && num < 0) {
        is_negative = true;
        num = -num;
    }

    // Convert the number to a string in reverse order
    do {
        buffer[index++] = digits[num % base];
        num /= base;
    } while (num > 0);

    // Add padding for precision
    while (index < precision) {
        buffer[index++] = '0';
    }

    // Add sign or space
    if (is_negative) {
        buffer[index++] = '-';
    } else if (flags & FLAG_PLUS) {
        buffer[index++] = '+';
    } else if (flags & FLAG_SPACE) {
        buffer[index++] = ' ';
    }

    // Add special prefixes for hex or octal
    if (flags & FLAG_SPECIAL) {
        if (base == 16) {
            buffer[index++] = (flags & FLAG_SMALL) ? 'x' : 'X';
            buffer[index++] = '0';
        } else if (base == 8) {
            buffer[index++] = '0';
        }
    }

    // Reverse the buffer to get the correct order
    for (int i = 0; i < index / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[index - i - 1];
        buffer[index - i - 1] = temp;
    }

    // Add padding for width
    int padding = width - index;
    if (padding > 0 && !(flags & FLAG_LEFT)) {
        char pad_char = (flags & FLAG_ZEROPAD) ? '0' : ' ';
        while (padding-- > 0) {
            *str++ = pad_char;
        }
    }

    // Copy the buffer to the output string
    for (int i = 0; i < index; i++) {
        *str++ = buffer[i];
    }

    // Add padding for left-justified numbers
    if (padding > 0 && (flags & FLAG_LEFT)) {
        while (padding-- > 0) {
            *str++ = ' ';
        }
    }

    return str;
}

// Custom vsprintf implementation
int vsprintf(char *buf, const char *fmt, va_list args) {
    char *str = buf;

    while (*fmt) {
        if (*fmt != '%') {
            *str++ = *fmt++;
            continue;
        }

        // Parse flags
        int flags = 0;
        while (true) {
            fmt++;
            switch (*fmt) {
                case '-': flags |= FLAG_LEFT; break;
                case '+': flags |= FLAG_PLUS; break;
                case ' ': flags |= FLAG_SPACE; break;
                case '#': flags |= FLAG_SPECIAL; break;
                case '0': flags |= FLAG_ZEROPAD; break;
                default: goto done_flags;
            }
        }
        done_flags:

        // Parse field width
        int width = -1;
        if (is_digit(*fmt)) {
            width = skip_atoi(&fmt);
        } else if (*fmt == '*') {
            width = va_arg(args, int);
            fmt++;
        }

        // Parse precision
        int precision = -1;
        if (*fmt == '.') {
            fmt++;
            if (is_digit(*fmt)) {
                precision = skip_atoi(&fmt);
            } else if (*fmt == '*') {
                precision = va_arg(args, int);
                fmt++;
            }
        }

        // Parse length modifier (ignored in this implementation)
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
            fmt++;
        }

        // Handle format specifiers
        switch (*fmt) {
            case 'c': {
                char c = (char) va_arg(args, int);
                *str++ = c;
                break;
            }
            case 's': {
                const char *s = va_arg(args, const char *);
                while (*s) {
                    *str++ = *s++;
                }
                break;
            }
            case 'd':
            case 'i': {
                int num = va_arg(args, int);
                str = number_to_string(str, num, 10, width, precision, flags | FLAG_PLUS);
                break;
            }
            case 'u': {
                unsigned int num = va_arg(args, unsigned int);
                str = number_to_string(str, num, 10, width, precision, flags);
                break;
            }
            case 'o': {
                unsigned int num = va_arg(args, unsigned int);
                str = number_to_string(str, num, 8, width, precision, flags);
                break;
            }
            case 'x':
            case 'X': {
                unsigned int num = va_arg(args, unsigned int);
                if (*fmt == 'x') {
                    flags |= FLAG_SMALL;
                }
                str = number_to_string(str, num, 16, width, precision, flags);
                break;
            }
            case 'p': {
                void *ptr = va_arg(args, void *);
                str = number_to_string(str, (uintptr_t) ptr, 16, width, precision, flags | FLAG_SPECIAL);
                break;
            }
            case '%': {
                *str++ = '%';
                break;
            }
            default: {
                *str++ = '%';
                *str++ = *fmt;
                break;
            }
        }
        fmt++;
    }

    *str = '\0';  // Null-terminate the string
    return str - buf;  // Return the number of characters written
}
#include <kernel.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

// Device driver table
static struct {
    device_write_fn write;
} fd_table[3] = {
    { NULL },  // stdin (not implemented)
    { NULL },  // stdout
    { NULL }   // stderr
};

// Actual console write implementation
static ssize_t console_write(int fd, const void *buf, size_t count)
{
    // Your kernel's actual output mechanism (VGA, serial, etc.)
    // This is a simple VGA text mode implementation example
    static unsigned short *vga_buffer = (unsigned short *)0xB8000;
    static int cursor_x = 0, cursor_y = 0;
    const char *str = buf;
    
    for (size_t i = 0; i < count; i++) {
        if (str[i] == '\n') {
            cursor_x = 0;
            if (++cursor_y >= 25) {
                cursor_y = 24;
                // TODO: Implement screen scrolling
            }
        } else {
            vga_buffer[cursor_y * 80 + cursor_x] = (0x0F << 8) | str[i];
            if (++cursor_x >= 80) {
                cursor_x = 0;
                if (++cursor_y >= 25) {
                    cursor_y = 24;
                    // TODO: Implement screen scrolling
                }
            }
        }
    }
    
    return count;  // Assume all bytes written successfully
}

ssize_t write(int fd, const void *buf, size_t count)
{
    if (fd < 0 || fd >= (int)(sizeof(fd_table)/sizeof(fd_table[0]))) {
        errno = EBADF;
        return -1;
    }

    if (!fd_table[fd].write) {
        errno = EBADF;
        return -1;
    }

    if (!buf) {
        errno = EFAULT;
        return -1;
    }

    if (count == 0) return 0;

    ssize_t result = fd_table[fd].write(fd, buf, count);
    
    if (result < 0) {
        // Translate device-specific errors to standard errno
        errno = EIO;
        return -1;
    }

    return result;
}

void kernel_io_init(void)
{
    // Initialize standard output devices
    fd_table[1].write = console_write;  // stdout
    fd_table[2].write = console_write;  // stderr
}
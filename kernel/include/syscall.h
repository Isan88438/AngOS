#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_syscall(void);
void sys_handler(void);
void enter_userspace(void *RIP, void *RSP);

void debug_log(char *format, ...);
void error(char *format, ...);
void memdump(const void *ptr, size_t count);

unsigned int logs(char *str);
void logchar(char c);

#ifdef __cplusplus
}
#endif

#endif

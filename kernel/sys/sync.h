#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stdint.h>

typedef struct {
    volatile uint8_t lock; // 0 = unlocked, 1 = locked
} kmutex_t;

void kmutex_init(kmutex_t *mutex);
void kmutex_lock(kmutex_t *mutex);
void kmutex_unlock(kmutex_t *mutex);

typedef struct {
    volatile int count;
    kmutex_t lock;
} ksema_t;
 
void ksema_init(ksema_t *sema, int initial_count);
void ksema_down(ksema_t *sema);
void ksema_up(ksema_t *sema);

#ifdef __cplusplus
}
#endif
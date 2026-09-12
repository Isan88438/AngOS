#include "sync.h"

void kmutex_init(kmutex_t *mutex) {
    mutex->lock = 0;
}

void kmutex_lock(kmutex_t *mutex) {
    while (__sync_lock_test_and_set(&mutex->lock, 1)) {
        // Spin until the lock is acquired
        __asm__ volatile ("pause");
    }
}

void kmutex_unlock(kmutex_t *mutex) {
    __sync_lock_release(&mutex->lock);
}

void ksema_init(ksema_t *sema, int initial_count) {
    sema->count = initial_count;
    kmutex_init(&sema->lock);
}

void ksema_down(ksema_t *sema) {
    while (1) {
        kmutex_lock(&sema->lock);
        if (sema->count > 0) {
            sema->count--;
            kmutex_unlock(&sema->lock);
            return;
        }
        kmutex_unlock(&sema->lock);
        // Sleep or yield the CPU
        __asm__ volatile ("pause");
    }
}

void ksema_up(ksema_t *sema) {
    kmutex_lock(&sema->lock);
    sema->count++;
    kmutex_unlock(&sema->lock);
}
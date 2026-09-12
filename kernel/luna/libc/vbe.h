#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stdint.h>

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA  0x01CF

extern void outw(uint16_t port, uint16_t value);

extern void vbe_write(uint16_t index, uint16_t value);

#ifdef __cplusplus
}
#endif
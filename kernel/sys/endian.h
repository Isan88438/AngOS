#ifndef _SYS_ENDIAN_H
#define _SYS_ENDIAN_H

#include <stdint.h>

// Host to little-endian (for x86, which is little-endian)
#define htole16(x) (x)
#define htole32(x) (x)

// Little-endian to host (for x86, which is little-endian)
#define le16toh(x) (x)
#define le32toh(x) (x)

#endif // _SYS_ENDIAN_H
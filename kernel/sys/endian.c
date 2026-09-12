// endian.c (for big-endian systems)
// uint32_t le32toh(uint32_t x) {
//     return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24);
// }
#pragma once
#include <stdint.h>

namespace glcd {

constexpr uint64_t glyph64(uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t r5, uint8_t r6, uint8_t r7) {
    return ((uint64_t)r0 << 0) |
           ((uint64_t)r1 << 8) |
           ((uint64_t)r2 << 16) |
           ((uint64_t)r3 << 24) |
           ((uint64_t)r4 << 32) |
           ((uint64_t)r5 << 40) |
           ((uint64_t)r6 << 48) |
           ((uint64_t)r7 << 56);
}

inline void glyph64Bit(uint64_t& glyph, uint8_t x, uint8_t y, bool value) {
    if (x >= 5 || y >= 8) return;

    uint8_t bit = y * 8 + (4 - x);

    if (value) glyph |= (uint64_t)1 << bit;
    else glyph &= ~((uint64_t)1 << bit);
}

}  // namespace glcd

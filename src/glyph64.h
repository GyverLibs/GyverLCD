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

}  // namespace glcd

#pragma once
#include <stddef.h>
#include <stdint.h>

namespace glcd {

struct UTFDecoder2 {
    int16_t decode(uint8_t byte) {
        if (_prev) {
            uint8_t prev = _prev;
            _prev = 0;

            if ((byte & 0xC0) == 0x80) {
                return ((prev & 0x1F) << 6) | (byte & 0x3F);
            }
        }

        if (byte < 0x80) return byte;

        if (byte >= 0xC2 && byte <= 0xDF) {
            _prev = byte;
        }

        return -1;
    }

    void reset() {
        _prev = 0;
    }

   private:
    uint8_t _prev = 0;
};

}  // namespace glcd
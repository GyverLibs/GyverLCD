#pragma once
#include <Arduino.h>

namespace glcd {

struct BitmapGlyph {
    char16_t utf;
    uint8_t bitmap[5];
};

struct RomGlyph {
    char16_t utf;
    uint8_t code;
};

struct GlyphTables {
    const RomGlyph* rom;
    const BitmapGlyph* bitmap;
};

struct GlyphRef {
    const uint8_t* bitmap;
    uint8_t code;
};

GlyphRef findGlyph(uint16_t code, GlyphTables tables);

}  // namespace glcd

extern const glcd::GlyphTables GTABLE_CYR_SOFT;
extern const glcd::GlyphTables GTABLE_CYR_HARD;

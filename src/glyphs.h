#pragma once
#include <Arduino.h>

namespace glcd {

int16_t mapGlyph(uint16_t code);
const uint8_t* getGlyph(uint8_t index);

}  // namespace glcd

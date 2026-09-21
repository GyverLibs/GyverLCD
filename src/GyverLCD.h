#pragma once
#include <Wire.h>

#include "GyverLCD_CORE.h"
#include "glyph64.h"
#include "lcdprint.h"

// стандартный Wire без Arduino Print
class GyverLCD_CORE : public GyverLCD_CORE_EXT<TwoWire> {
   public:
    GyverLCD_CORE(uint8_t address, uint8_t columns, uint8_t rows) : GyverLCD_CORE_EXT<TwoWire>(Wire, address, columns, rows) {}
};

// стандартный Wire + Arduino Print
using GyverLCD = GyverLCD_Print<GyverLCD_CORE>;

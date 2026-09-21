#pragma once
#include "./decoder.h"
#include "./glyphs.h"
#include "GyverLCD.h"

// MARK: GyverLCD_UTF8_BASE
template <typename LCD>
class GyverLCD_UTF8_BASE : public LCD {
   public:
    template <typename... Args>
    GyverLCD_UTF8_BASE(glcd::GlyphTables glyphTables, Args&&... args) : LCD((Args&&)args...), _glyphTables(glyphTables) {}

    size_t write(uint8_t data) {
        // !! IMPORTANT !!
        // return 1 нужен чтобы не ломался Arduino Print
        int16_t code = _decoder.decode(data);
        if (code < 0) return 1;
        if (code < 128) return LCD::write((uint8_t)code);

        glcd::GlyphRef glyph = glcd::findGlyph((uint16_t)code, _glyphTables);

        if (glyph.code) return LCD::write(glyph.code);
        if (!glyph.bitmap) return 1;
        if (!_glyphSlots) return 1;

        for (uint8_t i = 0; i < _glyphSlots; ++i) {
            if (_chars[i] == (uint16_t)code) return LCD::write(i);
        }

        uint8_t slot = _idx;
        LCD::createCharCols_P(slot, glyph.bitmap);
        _chars[slot] = (uint16_t)code;

        if (++_idx >= _glyphSlots) _idx = 0;
        return LCD::write(slot);
    }

    // вывести int как один байт через UTF-8 декодер
    size_t write(int data) {
        return write((uint8_t)data);
    }

    // вывести буфер через UTF-8 декодер
    size_t write(const uint8_t* data, size_t length) {
        if (!data) return 0;
        size_t n = 0;
        while (length--) n += write(*data++);
        return n;
    }

    // вывести строковый буфер через UTF-8 декодер
    size_t write(const char* data, size_t length) {
        return write((const uint8_t*)data, length);
    }

    // вывести строку через UTF-8 декодер до нулевого символа
    size_t write(const char* str) {
        if (!str) return 0;

        size_t n = 0;
        while (*str) n += write((uint8_t)*str++);
        return n;
    }

    // сбросить UTF-8 декодер и кеш глифов
    void resetUTF8() {
        _decoder.reset();
        resetGlyphs();
    }

    // сбросить кеш загруженных глифов
    void resetGlyphs() {
        for (uint8_t i = 0; i < 8; ++i) _chars[i] = 0;
        _idx = 0;
    }

    // задать количество слотов CGRAM для UTF-8: 0..8
    void setGlyphSlots(uint8_t slots) {
        _glyphSlots = slots <= 8 ? slots : 8;
        resetGlyphs();
    }

    // количество слотов CGRAM для UTF-8
    uint8_t glyphSlots() const {
        return _glyphSlots;
    }

   private:
    glcd::GlyphTables _glyphTables;
    glcd::UTFDecoder2 _decoder;
    uint16_t _chars[8] = {};
    uint8_t _glyphSlots = 8;
    uint8_t _idx = 0;
};

// MARK: GyverLCD_UTF8
class GyverLCD_UTF8 : public GyverLCD_Print<GyverLCD_UTF8_BASE<GyverLCD_CORE>> {
   public:
    using Base = GyverLCD_Print<GyverLCD_UTF8_BASE<GyverLCD_CORE>>;

    GyverLCD_UTF8(uint8_t address, uint8_t cols, uint8_t rows, glcd::GlyphTables glyphTables = GTABLE_CYR_SOFT)
        : Base(glyphTables, address, cols, rows) {}
};

// MARK: GyverLCD_UTF8_EXT
template <typename WireT>
class GyverLCD_UTF8_EXT : public GyverLCD_Print<GyverLCD_UTF8_BASE<GyverLCD_CORE_EXT<WireT>>> {
   public:
    using Base = GyverLCD_Print<GyverLCD_UTF8_BASE<GyverLCD_CORE_EXT<WireT>>>;

    GyverLCD_UTF8_EXT(WireT& wire, uint8_t address, uint8_t cols, uint8_t rows, glcd::GlyphTables glyphTables = GTABLE_CYR_SOFT)
        : Base(glyphTables, wire, address, cols, rows) {}
};

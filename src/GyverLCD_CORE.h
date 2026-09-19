#pragma once
#include <Arduino.h>

#include "./lcdpins.h"
#include "./lcdtimings.h"

// MARK: GyverLCD_CORE_EXT
template <class WireT>
class GyverLCD_CORE_EXT {
   public:
    GyverLCD_CORE_EXT(WireT& wire, uint8_t address, uint8_t columns, uint8_t rows) : _wire(wire), _address(address), _cols(columns), _rows(rows) {
        _rowOffsets[0] = 0x00;
        _rowOffsets[1] = rows > 1 ? 0x40 : 0;
        _rowOffsets[2] = rows > 2 ? columns : 0;
        _rowOffsets[3] = rows > 3 ? 0x40 + columns : 0;
    }

    // инициализировать expander и дисплей, шина I2C должна быть запущена заранее
    bool begin() {
        if (!_cols || _cols > 64 || !_rows || _rows > 4) return false;

        _displayControl = GLCD_DISPLAY_ON;
        _displayMode = GLCD_ENTRY_LEFT;
        _cursorCol = 0;
        _cursorRow = 0;

        // переводим expander в безопасное состояние до инициализации дисплея
        _writePort(_base(false));
        delay(50);

        // приводим LCD в известное состояние + в 4-битный режим
        _initNibble(0x03);
        delayMicroseconds(5000);
        _initNibble(0x03);
        delayMicroseconds(150);
        _initNibble(0x03);
        delayMicroseconds(150);
        _initNibble(0x02);
        delayMicroseconds(150);

        _command(GLCD_FUNCTION_SET | (_rows > 1 ? GLCD_2_LINE : 0));
        _command(GLCD_DISPLAY_CONTROL);
        _command(GLCD_CLEAR_DISPLAY);
        _command(GLCD_ENTRY_MODE_SET | _displayMode);
        _command(GLCD_DISPLAY_CONTROL | _displayControl);
        waitReady();
        _setFlag(Begun, true);
        return true;
    }

    // сообщить частоту I2C для оптимизации задержек, настройки самой шины не меняет
    void setBusClock(uint32_t hz) {
        _setFlag(NoCommandWait, hz && hz <= GLCD_NO_WAIT_MAX_HZ);
    }

    // I2C адрес дисплея
    uint8_t address() const {
        return _address;
    }

    // количество колонок дисплея
    uint8_t cols() const {
        return _cols;
    }

    // количество строк дисплея
    uint8_t rows() const {
        return _rows;
    }

    // задать адреса начала строк DDRAM для нестандартной геометрии дисплея
    void setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2 = 0, uint8_t row3 = 0) {
        _rowOffsets[0] = row0;
        _rowOffsets[1] = row1;
        _rowOffsets[2] = row2;
        _rowOffsets[3] = row3;
    }

    // очистить экран и вернуть курсор в начало
    void clear() {
        _command(GLCD_CLEAR_DISPLAY);
        _cursorCol = _cursorRow = 0;
    }

    // вернуть курсор и сдвиг дисплея в исходное положение
    void home() {
        _command(GLCD_RETURN_HOME);
        _cursorCol = _cursorRow = 0;
    }

    // установить курсор в колонку и строку
    void setCursor(uint8_t col, uint8_t row) {
        if (row >= _rows) row = _rows - 1;
        if (col == _cursorCol && row == _cursorRow) return;
        _command(GLCD_SET_DDRAM_ADDR | (_rowOffsets[row] + col));
        _cursorCol = col;
        _cursorRow = row;
    }

    // установить колонку курсора
    void setCursorCol(uint8_t col) {
        if (_cursorRow != UNKNOWN_ROW) setCursor(col, _cursorRow);
    }

    // установить строку курсора
    void setCursorRow(uint8_t row) {
        if (_cursorRow != UNKNOWN_ROW) setCursor(_cursorCol, row);
    }

    // перевести курсор в начало следующей строки, при text loop последняя строка переходит в первую
    void newLine() {
        if (_cursorRow == UNKNOWN_ROW) return;
        uint8_t row = _cursorRow;
        if (row + 1 < _rows) ++row;
        else if (_flag(TextLoop)) row = 0;
        setCursor(0, row);
    }

    // очистить текущую строку и вернуть курсор в её начало
    void clearLine() {
        _clearLine(0, true);
    }

    // очистить текущую строку от позиции курсора до конца, курсор оставить где был
    void clearEnd() {
        _clearLine(_cursorCol, false);
    }

    // текущая колонка курсора
    uint8_t cursorCol() const {
        return _cursorCol;
    }

    // текущая строка курсора (или 0xff, если неизвестна)
    uint8_t cursorRow() const {
        return _cursorRow;
    }

    // автоматический перенос на следующую строку при переполнении
    void setTextWrap(bool enabled) {
        _setFlag(TextWrap, enabled);
    }

    // состояние автоматического переноса строки
    bool isTextWrap() const {
        return _flag(TextWrap);
    }

    // переход с последней строки на первую
    void setTextLoop(bool enabled) {
        _setFlag(TextLoop, enabled);
    }

    // возвращает состояние кольцевого перехода строк
    bool isTextLoop() const {
        return _flag(TextLoop);
    }

    // вывести один байт с логическим переводом и переносом строк
    size_t write(uint8_t value) {
        if (_cursorRow != UNKNOWN_ROW) {
            if (value == '\r') return 1;
            if (value == '\n') {
                newLine();
                return 1;
            }
            if (_cursorCol >= _cols) {
                if (!_flag(TextWrap)) return 1;

                if (_cursorRow + 1 < _rows) setCursor(0, _cursorRow + 1);
                else if (_flag(TextLoop)) setCursor(0, 0);
                else return 1;
            }
        }

        _data(value);
        if (_cursorRow != UNKNOWN_ROW) _cursorCol += (_displayMode & GLCD_ENTRY_LEFT) ? 1 : -1;
        return 1;
    }

    // вывести int как один байт, в том числе write(0)
    size_t write(int value) {
        return write((uint8_t)value);
    }

    // вывести буфер байт
    size_t write(const uint8_t* data, size_t length) {
        if (!data) return 0;

        size_t n = 0;
        while (length--) n += write(*data++);
        return n;
    }

    // вывести строковый буфер
    size_t write(const char* data, size_t length) {
        return write((const uint8_t*)data, length);
    }

    // вывести строку до нулевого символа
    size_t write(const char* str) {
        if (!str) return 0;

        size_t n = 0;
        while (*str) n += write((uint8_t)*str++);
        return n;
    }

    // состояние отображения
    void setDisplay(bool enabled) {
        _setDisplay(GLCD_DISPLAY_ON, enabled);
    }

    // состояние отображения
    bool isDisplay() const {
        return _displayControl & GLCD_DISPLAY_ON;
    }

    // видимость курсора
    void setCursorVisible(bool enabled) {
        _setDisplay(GLCD_CURSOR_ON, enabled);
    }

    // видимость курсора
    bool isCursorVisible() const {
        return _displayControl & GLCD_CURSOR_ON;
    }

    // мигание курсора
    void setBlink(bool enabled) {
        _setDisplay(GLCD_BLINK_ON, enabled);
    }

    // состояние мигания курсора
    bool isBlink() const {
        return _displayControl & GLCD_BLINK_ON;
    }

    // движение адреса слева направо после записи символа
    void leftToRight() {
        _setEntry(GLCD_ENTRY_LEFT, true);
    }

    // движение адреса справа налево после записи символа
    void rightToLeft() {
        _setEntry(GLCD_ENTRY_LEFT, false);
    }

    // направление движения адреса после записи символа
    bool isLeftToRight() const {
        return _displayMode & GLCD_ENTRY_LEFT;
    }

    // автоматический сдвиг дисплея
    void setAutoscroll(bool enabled) {
        _setEntry(GLCD_ENTRY_SHIFT_INCREMENT, enabled);
    }

    // состояние автоматического сдвига дисплея
    bool isAutoscroll() const {
        return _displayMode & GLCD_ENTRY_SHIFT_INCREMENT;
    }

    // изображение на одну позицию влево
    void scrollDisplayLeft() {
        _command(GLCD_CURSOR_SHIFT | GLCD_DISPLAY_MOVE);
    }

    // изображение на одну позицию вправо
    void scrollDisplayRight() {
        _command(GLCD_CURSOR_SHIFT | GLCD_DISPLAY_MOVE | GLCD_MOVE_RIGHT);
    }

    // состояние подсветки
    void setBacklight(bool enabled) {
        _setFlag(Backlight, enabled);
        if (_flag(Begun)) _writePort(_base(false));
    }

    // состояние подсветки
    bool isBacklight() const {
        return _flag(Backlight);
    }

    // создать пользовательский символ 0..7 из 8 строк по 5 бит
    void createChar(uint8_t index, const uint8_t bitmap[8]) {
        if (!bitmap) return;

        _command(GLCD_SET_CGRAM_ADDR | ((index & 0x07) << 3));
        for (uint8_t i = 0; i < 8; ++i) _data(bitmap[i]);

        if (_cursorRow != UNKNOWN_ROW) {
            _command(GLCD_SET_DDRAM_ADDR | (_rowOffsets[_cursorRow] + _cursorCol));
        }
    }

    // progmem версия
    void createChar_P(uint8_t index, const uint8_t bitmapP[8]) {
        uint8_t bitmap[8];
        memcpy_P(bitmap, bitmapP, 8);
        createChar(index, bitmap);
    }

    // создать пользовательский символ 0..7 из 5 колонок по 8 бит
    void createCharCols(uint8_t index, const uint8_t bitmap[5]) {
        if (!bitmap) return;

        _command(GLCD_SET_CGRAM_ADDR | ((index & 0x07) << 3));

        for (uint8_t y = 0; y < 8; ++y) {
            uint8_t line = 0;

            for (uint8_t x = 0; x < 5; ++x) {
                if (bitmap[x] & (1 << y)) line |= 1 << (4 - x);
            }

            _data(line);
        }

        if (_cursorRow != UNKNOWN_ROW) {
            _command(GLCD_SET_DDRAM_ADDR | (_rowOffsets[_cursorRow] + _cursorCol));
        }
    }

    // progmem версия
    void createCharCols_P(uint8_t index, const uint8_t bitmapP[5]) {
        uint8_t bitmap[5];
        memcpy_P(bitmap, bitmapP, 5);
        createCharCols(index, bitmap);
    }

    // низкоуровневая команда HD44780 и сброс позиции курсора
    void command(uint8_t value) {
        _command(value);
        _cursorRow = UNKNOWN_ROW;
    }

   private:
    // дождаться окончания последней отправленной обычной команды
    void waitReady() {
        if (_flag(NoCommandWait)) delayMicroseconds(GLCD_COMMAND_US);
    }

    static constexpr uint8_t GLCD_CLEAR_DISPLAY = 0x01;
    static constexpr uint8_t GLCD_RETURN_HOME = 0x02;
    static constexpr uint8_t GLCD_ENTRY_MODE_SET = 0x04;
    static constexpr uint8_t GLCD_DISPLAY_CONTROL = 0x08;
    static constexpr uint8_t GLCD_CURSOR_SHIFT = 0x10;
    static constexpr uint8_t GLCD_FUNCTION_SET = 0x20;
    static constexpr uint8_t GLCD_SET_CGRAM_ADDR = 0x40;
    static constexpr uint8_t GLCD_SET_DDRAM_ADDR = 0x80;
    static constexpr uint8_t GLCD_ENTRY_LEFT = 0x02;
    static constexpr uint8_t GLCD_ENTRY_SHIFT_INCREMENT = 0x01;
    static constexpr uint8_t GLCD_DISPLAY_ON = 0x04;
    static constexpr uint8_t GLCD_CURSOR_ON = 0x02;
    static constexpr uint8_t GLCD_BLINK_ON = 0x01;
    static constexpr uint8_t GLCD_DISPLAY_MOVE = 0x08;
    static constexpr uint8_t GLCD_MOVE_RIGHT = 0x04;
    static constexpr uint8_t GLCD_2_LINE = 0x08;
    static constexpr uint8_t UNKNOWN_ROW = 0xFF;

    enum Flag : uint8_t {
        Begun = 1 << 0,
        Backlight = 1 << 1,
        TextWrap = 1 << 2,
        TextLoop = 1 << 3,
        NoCommandWait = 1 << 4,
    };

    static constexpr uint8_t _pinMask(uint8_t pin) {
        return pin < 8 ? 1u << pin : 0;
    }

    struct Masks {
        static constexpr uint8_t rs = _pinMask(GLCD_PIN_RS);
        static constexpr uint8_t en = _pinMask(GLCD_PIN_EN);
        static constexpr uint8_t d4 = _pinMask(GLCD_PIN_D4);
        static constexpr uint8_t d5 = _pinMask(GLCD_PIN_D5);
        static constexpr uint8_t d6 = _pinMask(GLCD_PIN_D6);
        static constexpr uint8_t d7 = _pinMask(GLCD_PIN_D7);
        static constexpr uint8_t bl = _pinMask(GLCD_PIN_BL);
    };

    WireT& _wire;
    uint8_t _address;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _rowOffsets[4];
    uint8_t _cursorCol = 0;
    uint8_t _cursorRow = UNKNOWN_ROW;
    uint8_t _displayControl = GLCD_DISPLAY_ON;
    uint8_t _displayMode = GLCD_ENTRY_LEFT;
    uint8_t _flags = Backlight | TextWrap;

    bool _flag(Flag flag) const {
        return _flags & flag;
    }

    void _setFlag(Flag flag, bool value) {
        if (value) _flags |= flag;
        else _flags &= ~flag;
    }

    void _setEntry(uint8_t flag, bool value) {
        if (value) _displayMode |= flag;
        else _displayMode &= ~flag;
        _command(GLCD_ENTRY_MODE_SET | _displayMode);
    }

    void _setDisplay(uint8_t flag, bool value) {
        if (value) _displayControl |= flag;
        else _displayControl &= ~flag;
        _command(GLCD_DISPLAY_CONTROL | _displayControl);
    }

    void _clearLine(uint8_t col, bool toStart) {
        if (_cursorRow == UNKNOWN_ROW || col >= _cols) return;
        uint8_t row = _cursorRow;
        uint8_t mode = _displayMode;
        uint8_t clearMode = (mode | GLCD_ENTRY_LEFT) & ~GLCD_ENTRY_SHIFT_INCREMENT;

        // очищаем слева направо без автосдвига независимо от текущего entry mode
        if (clearMode != mode) {
            _displayMode = clearMode;
            _command(GLCD_ENTRY_MODE_SET | _displayMode);
        }

        setCursor(col, row);
        while (_cursorCol < _cols) write(' ');
        if (_displayMode != mode) {
            _displayMode = mode;
            _command(GLCD_ENTRY_MODE_SET | _displayMode);
        }
        setCursor(toStart ? 0 : col, row);
    }

    uint8_t _mapNibble(uint8_t nibble) const {
#if GLCD_PIN_D4 == 4 && GLCD_PIN_D5 == 5 && GLCD_PIN_D6 == 6 && GLCD_PIN_D7 == 7
        return nibble << 4;
#elif GLCD_PIN_D4 == 0 && GLCD_PIN_D5 == 1 && GLCD_PIN_D6 == 2 && GLCD_PIN_D7 == 3
        return nibble & 0x0F;
#else
        uint8_t out = 0;
        if (nibble & 0x01) out |= Masks::d4;
        if (nibble & 0x02) out |= Masks::d5;
        if (nibble & 0x04) out |= Masks::d6;
        if (nibble & 0x08) out |= Masks::d7;
        return out;
#endif
    }

    uint8_t _base(bool rs) const {
        uint8_t out = rs ? Masks::rs : 0;
        if (Masks::bl && (_flag(Backlight) == GLCD_BL_ACTIVE_HIGH)) out |= Masks::bl;
        return out;
    }

    void _writePort(uint8_t value) {
        _wire.beginTransmission(_address);
        _wire.write(value);
        _wire.endTransmission();
    }

    void _initNibble(uint8_t nibble) {
        uint8_t data = _base(false) | _mapNibble(nibble);
        _wire.beginTransmission(_address);
        _wire.write(data & ~Masks::en);
        _wire.write(data | Masks::en);
        _wire.write(data & ~Masks::en);
        _wire.endTransmission();
    }

    void _command(uint8_t value) {
        _send(value, false);
        if (value == GLCD_CLEAR_DISPLAY || value == GLCD_RETURN_HOME) delayMicroseconds(GLCD_CLEAR_US);
        else if (!_flag(NoCommandWait)) delayMicroseconds(GLCD_COMMAND_US);
    }

    void _data(uint8_t value) {
        _send(value, true);
        if (!_flag(NoCommandWait)) delayMicroseconds(GLCD_COMMAND_US);
    }

    void _send(uint8_t value, bool rs) {
        uint8_t base = _base(rs);
        uint8_t hi = base | _mapNibble(value >> 4);
        uint8_t lo = base | _mapNibble(value);

        _wire.beginTransmission(_address);
        _wire.write(hi & ~Masks::en);
        _wire.write(hi | Masks::en);
        _wire.write(hi & ~Masks::en);
        _wire.write(lo & ~Masks::en);
        _wire.write(lo | Masks::en);
        _wire.write(lo & ~Masks::en);
        _wire.endTransmission();
    }
};

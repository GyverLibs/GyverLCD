#include <GyverLCD.h>

GyverLCD lcd(0x27, 16, 2);

// встроенный 64-бит редактор
uint64_t smile64 = glcd::glyph64(
    0b00000,
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000);

void setup() {
    Wire.begin();
    lcd.begin();
    lcd.print("Custom: ");

    // редактор символов есть в папке utils

    // 1. классический вариант - 8 байт строками
    const uint8_t smile[8] = {0x00, 0x0a, 0x0a, 0x00, 0x11, 0x0e, 0x00, 0x00};
    lcd.createChar(0, smile);
    lcd.write(0);

    // 2. самый лёгкий вариант - 8 байт строками, упакованы в 64 бит
    lcd.createChar(1, 0x40a110a0400);
    lcd.write(1);

    // можно сохранить и использовать по имени
    lcd.createChar(2, smile64);
    lcd.write(2);

    // 3. компактный вариант - 5 байт столбцами (удобно для хранения в таблицах шрифтов)
    const uint8_t spiral[5] = {0x7d, 0x45, 0x5d, 0x41, 0x7f};
    lcd.createCharCols(3, spiral);
    lcd.write(3);
}

void loop() {
}

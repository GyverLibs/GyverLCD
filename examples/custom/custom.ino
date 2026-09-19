#include <GyverLCD.h>

GyverLCD lcd(0x27, 16, 2);

void setup() {
    Wire.begin();
    lcd.begin();
    lcd.print("Custom: ");

    // bitmaps
    const uint8_t smile[5] = {0x10, 0x24, 0x20, 0x24, 0x10};
    const uint8_t square[5] = {0x04, 0x0e, 0x1f, 0x0e, 0x04};
    const uint8_t spiral[5] = {0x7d, 0x45, 0x5d, 0x41, 0x7f};

    // загружаем в ячейки
    lcd.createCharCols(0, smile);
    lcd.createCharCols(1, square);
    lcd.createCharCols(2, spiral);

    // выводим
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);

    // наглядный пример
    uint8_t iconRows[8] = {
        0b00100,
        0b01010,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b00000,
    };
    lcd.createChar(3, iconRows);

    uint8_t iconCols[5] = {
        0b00011100,
        0b00100010,
        0b01000001,
        0b00100010,
        0b00011100,
    };
    lcd.createCharCols(4, iconCols);

    lcd.write(3);
    lcd.write(4);
}

void loop() {
}

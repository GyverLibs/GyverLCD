#include <GyverLCD_UTF8.h>

GyverLCD_UTF8 lcd(0x27, 16, 2);  // программная поддержка кириллицы
// GyverLCD_UTF8 lcd(0x27, 16, 2, GTABLE_CYR_HARD); // аппаратная поддержка кириллицы

void setup() {
    Wire.begin();
    lcd.begin();
    lcd.print("Привет, мир! 23°");
}

void loop() {
}

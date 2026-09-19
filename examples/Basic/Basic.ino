#include <GyverLCD.h>

GyverLCD lcd(0x27, 16, 2);

void setup() {
    Wire.begin();
    // разгон
    // Wire.setClock(400000);
    // lcd.setBusClock(400000);
    lcd.begin();
    lcd.println("GyverLCD");
    lcd.println("new line");
}

void loop() {
}

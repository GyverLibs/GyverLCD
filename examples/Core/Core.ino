#include <GyverLCD.h>

GyverLCD_CORE lcd(0x27, 16, 2);

void setup() {
    Wire.begin();
    lcd.begin();
    lcd.write("Hello core");
}

void loop() {
}

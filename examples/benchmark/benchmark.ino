#include <GyverLCD.h>
#include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcd(0x27, 20, 4);
// 100: 1074/1.5/1.5/3.5/14
// 400: same timing
// 800: same timing

GyverLCD lcd(0x27, 20, 4);
// 100: 64/0.25/0.8/2.8/7.8
// 400: 60/0.08/0.3/2.3/2.5
// 800: 60/0.07/0.2/2.2/2.1

uint32_t lap() {
    static uint32_t ms;
    uint32_t res = micros() - ms;
    ms = micros();
    return res;
}

void setup() {
    Wire.begin();
    // разгон
    const uint32_t clock = 100000;
    Wire.setClock(clock);

    lap();
    lcd.init();
    lcd.backlight();
    uint32_t init = lap();

    for (int i = 0; i < 10; i++) {
        lcd.setCursor(0, i);
    }
    uint32_t cursor = lap();

    for (int i = 0; i < 10; i++) {
        lcd.write('0' + i);
    }
    uint32_t print = lap();

    lcd.clear();
    uint32_t clear = lap();

    const char b[8] = {};
    lcd.createChar(0, b);

    uint32_t crchar = lap();

    lcd.home();
    lcd.print("in/cu/pr/cl/cr");
    lcd.setCursor(0, 1);
    lcd.print(init / 1000.0);
    lcd.print('/');
    lcd.print(cursor / 10 / 1000.0);
    lcd.setCursor(0, 2);
    lcd.print(print / 10 / 1000.0);
    lcd.print('/');
    lcd.print(clear / 1000.0);
    lcd.print('/');
    lcd.print(crchar / 1000.0);
}

void loop() {
}

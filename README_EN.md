This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverLCD.svg?color=brightgreen)](https://github.com/GyverLibs/GyverLCD/releases/latest/download/GyverLCD.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverLCD.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverLCD)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverLCD?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/%D0%9F%D0%9E%D0%94%D0%9F%D0%98%D0%A1%D0%90%D0%A2%D0%AC%D0%A1%D0%AF-%D0%9D%D0%90%20%D0%9E%D0%91%D0%9D%D0%9E%D0%92%D0%9B%D0%95%D0%9D%D0%98%D0%AF-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverLCD
Library for HD44780-compatible LCD displays with I2C backpack on PCF8574/PCF8574A/PCA8574(A)

- Lighter and 5-10 times faster than classic LCD libraries, well optimized timings
- Support for external Arduino Wire-like interface (for software or I2C implementation)
- Strict and understandable cursor behavior - transition to the next line, transfer by \n, looped output
- A rich and convenient API compared to the classics
- Version with software support for custom UTF-8 characters, built-in table for Cyrillic (RU+BY+UKR) - much lighter than analogues
- Support for several standard Chinese modules

## Initialization
### Regular Wire
Full version with Arduino`Print`:
```cpp
#include <GyverLCD.h>
GyverLCD lcd(0x27, 16, 2);
```

Lightweight core without Arduino`Print`:
```cpp
#include <GyverLCD.h>
GyverLCD_CORE lcd(0x27, 16, 2);
```

### External Wire-like interface
Full version with Arduino`Print`:
```cpp
#include <GyverLCD_EXT.h>
GyverLCD_EXT<MyWire> lcd(wire, 0x27, 16, 2);
```

Lightweight core without Arduino`Print`:
```cpp
#include <GyverLCD_CORE.h>
GyverLCD_CORE_EXT<MyWire> lcd(wire, 0x27, 16, 2);
```

### UTF-8
Standard.`Wire` + Arduino `Print`+ software glyphs RU/BY/UKR:
```cpp
#include <GyverLCD_UTF8.h>
GyverLCD_UTF8 lcd(0x27, 16, 2);
```

External Wire-like interface:
```cpp
#include <GyverLCD_UTF8.h>
GyverLCD_UTF8_EXT<MyWire> lcd(wire, 0x27, 16, 2);
```

If you need UTF-8 without Arduino`Print`The layer can be assembled directly on top of the core:
```cpp
GyverLCD_UTF8_BASE<GyverLCD_CORE> lcd(0x27, 16, 2);
GyverLCD_UTF8_BASE<GyverLCD_CORE_EXT<MyWire>> lcd(wire, 0x27, 16, 2);
```

`GyverLCD`and`GyverLCD_EXT`inherit the Arduino`Print`: available`print()`, `println()`numerical formats and other standard methods.`GyverLCD_CORE`and`GyverLCD_CORE_EXT`Contain only LCD API and lightweight`write()`virtual`Print`.

Library doesn't call`Wire.begin()`It does not change the I2C settings.

## API
```cpp
bool begin();                                             // initialize
void setBusClock(uint32_t hz);                            // report the actual I2C frequency (makes sense at >1 MHz)

uint8_t address() const;                                  // I2C address
uint8_t cols() const;                                     // columnage
uint8_t rows() const;                                     // lineage
void setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2 = 0, uint8_t row3 = 0); // DDRAM string addresses

void clear();                                             // clear out
void home();                                              // launch
void setCursor(uint8_t col, uint8_t row);                 // point
void setCursorCol(uint8_t col);                           // column
void setCursorRow(uint8_t row);                           // line up
uint8_t cursorCol() const;                                // column
uint8_t cursorRow() const;                                // current-line
void newLine();                                           // go to the beginning of the next line
void clearLine();                                         // Clear the current line and go to its beginning
void clearEnd();                                          // clear to the end of the line and return the cursor

size_t write(uint8_t value);                              // byte
size_t write(int value);                                  // output int as a byte, allows write(0)
size_t write(const uint8_t* data, size_t length);         // buffer
size_t write(const char* data, size_t length);            // line-buffer
size_t write(const char* str);                            // line up
// GyverLCD and GyverLCD EXT also inherit Arduino Print: print(), println(), etc.

void setTextWrap(bool enabled);                           // carry over
bool isTextWrap() const;                                  // transference
void setTextLoop(bool enabled);                           // after the last line go to the first
bool isTextLoop() const;                                  // circularity

void setDisplay(bool enabled);                            // turn on/off
bool isDisplay() const;                                   // mapping
void setCursorVisible(bool enabled);                      // point/hide
bool isCursorVisible() const;                             // cursorship
void setBlink(bool enabled);                              // flash off
bool isBlink() const;                                     // flashing

void leftToRight();                                       // left-right
void rightToLeft();                                       // right-left
bool isLeftToRight() const;                               // current
void setAutoscroll(bool enabled);                         // displayshift
bool isAutoscroll() const;                                // shift-state
void scrollDisplayLeft();                                 // move the image left
void scrollDisplayRight();                                // rightward

void setBacklight(bool enabled);                          // light up
bool isBacklight() const;                                 // background

void createChar(uint8_t index, const uint8_t bitmap[8]);       // symbol
void createChar_P(uint8_t index, const uint8_t bitmap[8]);     // PROGMEM version
void createCharCols(uint8_t index, const uint8_t bitmap[5]);   // symbol
void createCharCols_P(uint8_t index, const uint8_t bitmap[5]); // PROGMEM version

void command(uint8_t value);                              // low-level command HD44780

// UTF-8 version
void resetUTF8();                                         // drop off the decoder and glyph cache
void resetGlyphs();                                       // drop the CGRAM cache
void setGlyphSlots(uint8_t slots);                        // UTF-8 0.8 CGRAM slots
uint8_t glyphSlots() const;                               // number of UTF-8 slots
```

`GyverLCD_CORE`and`GyverLCD_CORE_EXT`They have the same LCD API but do not inherit Arduino.`Print`. `GyverLCD_UTF8_BASE<LCD>`Adds a UTF-8 decoder and CGRAM cache over any compatible core class.

## Work.
### Text and lines
- `\n`Translate the cursor to the beginning of the next visual line,`\r`ignored
- `setTextWrap(true)`transfers the text to the next visual line when overflowing,`false`redundancy
- `setTextLoop(true)`allows the transition from the last line to the first and`\n`and for automatic transfer
- The transfer is on the visual lines, not the natural course of DDRAM.
- `clearLine()`Clears the current line and puts the cursor at the beginning.
- `clearEnd()`Clears the current position to the end and returns the cursor

### Castomy symbols
The display has 8 cells under custom symbols. To display their symbols, they must be loaded into the desired cell and called`write(номер)`. In classical libraries, the format of the custom symbol is 8 bytes, are strings with pixels from top to bottom, loaded through`createChar`(for compatibility with the classics).

This library also uses a more compact version - 5 bytes, columns of pixels from left to right, loaded through`createCharCols`. The general mechanism is as follows:

```cpp
// classic, lines
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
lcd.createChar(0, iconRows);

// compact
uint8_t iconCols[5] = {
    0b00011100,
    0b00100010,
    0b01000001,
    0b00100010,
    0b00011100,
};
lcd.createCharCols(1, iconCols);

// conclusion
lcd.write(0);
lcd.write(1);
```

`createChar`It loads data into the display memory, i.e. the library does not store either this array or a pointer to it.

The library in the utils folder has a built-in utility for generating icons for custom symbols.

## Setting up
### UTF-8
The UTF-8 layer decodes single-byte and two-byte characters, uses similar CGROM symbols, and loads glyphs into CGRAM if necessary. By default, all 8 slots are available. The number of slots determines how many unique UTF-8 characters that do not match English can be on the display at the same time. When used above the current limit, the oldest characters will be overwritten. The display has only 8 cells for such symbols, all are used by default.

```cpp
lcd.setGlyphSlots(6);    // UTF-8 uses 0..5, 6..7 slots are free to createChar
lcd.print("Привет 23°C");
```

If the occupied UTF-8 slots are changed manually`createChar()`After that, you need to call`resetGlyphs()`.

### Pin map
The setting of the sling is in the internal file`lcdpins.h`. All define is given by the **do* connection header GyverLCD.

By default, the common wiring is used:
```text
P0 RS
P1 RW
P2 EN
P3 BL
P4 D4
P5 D5
P6 D6
P7 D7
```

Ready version:
```cpp
#define GLCD_MAP_MJKDZ
#include <GyverLCD.h>
```

Available`GLCD_MAP_LCM1602`, `GLCD_MAP_MJKDZ`, `GLCD_MAP_LCDXIO`.

For my wiring:
```cpp
#define GLCD_PIN_RS 6
#define GLCD_PIN_RW 5
#define GLCD_PIN_EN 4
#define GLCD_PIN_BL 7
#define GLCD_PIN_D4 0
#define GLCD_PIN_D5 1
#define GLCD_PIN_D6 2
#define GLCD_PIN_D7 3
#define GLCD_BL_ACTIVE_HIGH 0
#include <GyverLCD.h>
```

If the lighting is not connected to the expander:
```cpp
#define GLCD_PIN_BL GLCD_NO_PIN
```

### Taiming
By default, safe delays are used: 45 μs for a normal command and 2000 μs for a normal command.`clear/home`.

```cpp
#define GLCD_FAST
#include <GyverLCD.h>
```

Meanings:
```cpp
#define GLCD_COMMAND_US 50
#define GLCD_CLEAR_US 2200
#include <GyverLCD.h>
```

`setBusClock(hz)`When the library reports the actual frequency of I2C, at values greater than 1 MHz, the library will additionally wait a few microseconds for commands.
```cpp
Wire.setClock(1000000);
lcd.setBusClock(1000000);
```

### Benchmark (MS)
| Library | I2C | init | cursor | print | clear | create |
| ----------------- | ------: | ---: | -----: | ----: | ----: | -----: |
| LiquidCrystal I2C | 100 kHz | 1074 | 1.50 | 1.50 | 3.50 | 14.0 |
| LiquidCrystal I2C | 400 kHz | 1074 | 1.50 | 1.50 | 3.50 | 14.0 |
LiquidCrystal I2C | 800 kHz | 1074 | 1.50 | 1.50 | 3.50 | 14.0 |
| GyverLCD | 100 kHz | 64 | 0.25 | 0.80 | 2.80 | 7.8 |
| GyverLCD | 400 kHz | 60 | 0.08 | 0.30 | 2.30 | 2.5 |
| GyverLCD | 800 kHz | 60 | 0.07 | 0.20 | 2.20 | 2.1 |

## Example
```cpp
#include <GyverLCD.h>

GyverLCD lcd(0x27, 16, 2);

void setup() {
    Wire.begin();
    lcd.begin();
    lcd.print("Hello, World!");
}

void loop() {
}
```

<a id="install"></a>

## Installation
- The library can be found under the name **GyverLCD** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/GyverLCD/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.

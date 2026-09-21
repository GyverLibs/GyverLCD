[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverLCD.svg?color=brightgreen)](https://github.com/GyverLibs/GyverLCD/releases/latest/download/GyverLCD.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverLCD.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverLCD)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverLCD?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/%D0%9F%D0%9E%D0%94%D0%9F%D0%98%D0%A1%D0%90%D0%A2%D0%AC%D0%A1%D0%AF-%D0%9D%D0%90%20%D0%9E%D0%91%D0%9D%D0%9E%D0%92%D0%9B%D0%95%D0%9D%D0%98%D0%AF-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverLCD
Библиотека для символьных HD44780-совместимых LCD дисплеев с I2C backpack на PCF8574/PCF8574A/PCA8574(A)

- Легче и в 5-10 раз быстрее "классических" библиотек для LCD, хорошо оптимизированы тайминги
- Поддержка работы с внешним Arduino Wire-like интерфейсом (для программной или своей реализации I2C)
- Поддержка нескольких стандартных распиновок китайских модулей
- Строгое и понятное поведение курсора - переход на следующую строку, перенос по \n, зацикленный вывод
- Богатый и удобный API по сравнению с классикой
- Поддержка дисплеев с аппаратной кириллицей и другими языками
- Программная поддержка кастомных UTF-8 символов, встроенная таблица для кириллицы (RU+BY+UKR) - сильно легче аналогов
- Компактный режим создания кастомных символов + приложение-редактор иконок (также доступно по [ссылке](https://gyverlibs.github.io/GyverLCD/utils/IconEditor.html))

## Инициализация
### Обычный Wire
Полная версия с Arduino `Print`:
```cpp
#include <GyverLCD.h>
GyverLCD lcd(0x27, 16, 2);
```

Лёгкое ядро без Arduino `Print`:
```cpp
#include <GyverLCD.h>
GyverLCD_CORE lcd(0x27, 16, 2);
```

### Внешний Wire-like интерфейс
Полная версия с Arduino `Print`:
```cpp
#include <GyverLCD_EXT.h>
GyverLCD_EXT<MyWire> lcd(wire, 0x27, 16, 2);
```

Лёгкое ядро без Arduino `Print`:
```cpp
#include <GyverLCD_CORE.h>
GyverLCD_CORE_EXT<MyWire> lcd(wire, 0x27, 16, 2);
```

### UTF-8
Стандартный `Wire` + Arduino `Print`:
```cpp
#include <GyverLCD_UTF8.h>
GyverLCD_UTF8 lcd(0x27, 16, 2);
```

Внешний Wire-like интерфейс:
```cpp
#include <GyverLCD_UTF8.h>
GyverLCD_UTF8_EXT<MyWire> lcd(wire, 0x27, 16, 2);
```

Если нужен UTF-8 без Arduino `Print`, слой можно собрать напрямую поверх ядра:
```cpp
GyverLCD_UTF8_BASE<GyverLCD_CORE> lcd(0x27, 16, 2);
GyverLCD_UTF8_BASE<GyverLCD_CORE_EXT<MyWire>> lcd(wire, 0x27, 16, 2);
```

`GyverLCD` и `GyverLCD_EXT` наследуют Arduino `Print`: доступны `print()`, `println()`, числовые форматы и остальные стандартные методы. `GyverLCD_CORE` и `GyverLCD_CORE_EXT` содержат только LCD API и лёгкие `write()` без виртуального `Print`. Библиотека не вызывает `Wire.begin()` и не меняет настройки I2C.

## API
```cpp
bool begin();                                             // инициализировать дисплей
void setBusClock(uint32_t hz);                            // сообщить фактическую частоту I2C (имеет смысл при > 1 МГц)

uint8_t address() const;                                  // I2C адрес
uint8_t cols() const;                                     // количество колонок
uint8_t rows() const;                                     // количество строк
void setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2 = 0, uint8_t row3 = 0); // DDRAM адреса строк

void clear();                                             // очистить дисплей
void home();                                              // вернуть курсор в начало
void setCursor(uint8_t col, uint8_t row);                 // установить курсор
void setCursorCol(uint8_t col);                           // установить колонку
void setCursorRow(uint8_t row);                           // установить строку
uint8_t cursorCol() const;                                // текущая колонка
uint8_t cursorRow() const;                                // текущая строка или 0xFF
void newLine();                                           // перейти в начало следующей строки
void clearLine();                                         // очистить текущую строку и перейти в её начало
void clearEnd();                                          // очистить до конца строки и вернуть курсор

size_t write(uint8_t value);                              // вывести байт
size_t write(int value);                                  // вывести int как байт, позволяет write(0)
size_t write(const uint8_t* data, size_t length);         // вывести буфер байт
size_t write(const char* data, size_t length);            // вывести строковый буфер
size_t write(const char* str);                            // вывести строку до \0
// GyverLCD и GyverLCD_EXT также наследуют Arduino Print: print(), println() и т.д.

void setTextWrap(bool enabled);                           // переносить при переполнении строки
bool isTextWrap() const;                                  // состояние переноса
void setTextLoop(bool enabled);                           // после последней строки переходить на первую
bool isTextLoop() const;                                  // состояние кольцевого перехода

void setDisplay(bool enabled);                            // включить/выключить отображение
bool isDisplay() const;                                   // состояние отображения
void setCursorVisible(bool enabled);                      // показать/скрыть курсор
bool isCursorVisible() const;                             // видимость курсора
void setBlink(bool enabled);                              // включить/выключить мигание
bool isBlink() const;                                     // состояние мигания

void leftToRight();                                       // направление адреса слева направо
void rightToLeft();                                       // направление адреса справа налево
bool isLeftToRight() const;                               // текущее направление
void setAutoscroll(bool enabled);                         // автоматический сдвиг дисплея
bool isAutoscroll() const;                                // состояние автосдвига
void scrollDisplayLeft();                                 // сдвинуть изображение влево
void scrollDisplayRight();                                // сдвинуть изображение вправо

void setBacklight(bool enabled);                          // включить/выключить подсветку
bool isBacklight() const;                                 // состояние подсветки

void createChar(uint8_t index, uint64_t bitmap);               // символ из 8 строк по 5 бит
void createChar(uint8_t index, const uint8_t bitmap[8]);       // символ из 8 строк по 5 бит
void createChar_P(uint8_t index, const uint8_t bitmap[8]);     // PROGMEM версия
void createCharCols(uint8_t index, const uint8_t bitmap[5]);   // символ из 5 колонок по 8 бит
void createCharCols_P(uint8_t index, const uint8_t bitmap[5]); // PROGMEM версия

void command(uint8_t value);                              // низкоуровневая команда HD44780

// дополнительно в UTF-8 версии
void resetUTF8();                                         // сбросить декодер и кеш глифов
void resetGlyphs();                                       // сбросить кеш CGRAM
void setGlyphSlots(uint8_t slots);                        // выделить UTF-8 0..8 слотов CGRAM
uint8_t glyphSlots() const;                               // количество слотов UTF-8
```

## Работа
### Текст и строки
- `\n` переводит курсор в начало следующей визуальной строки, `\r` игнорируется
- `setTextWrap(true)` переносит текст на следующую визуальную строку при переполнении, при `false` лишние символы отбрасываются
- `setTextLoop(true)` разрешает переход с последней строки на первую и для `\n`, и для автоматического переноса
- Перенос идёт по визуальным строкам, а не по естественному ходу DDRAM дисплея (там может быть чередование)
- `clearLine()` очищает текущую строку и ставит курсор в её начало
- `clearEnd()` очищает от текущей позиции до конца и возвращает курсор обратно

### Свои символы и иконки
У дисплея есть 8 ячеек под кастомные символы. Для вывода своего символа его нужно однократно загрузить в желаемую ячейку и вызвать `write(номер ячейки)`. После вывода символ остаётся в памяти, т.е. загружать его каждый раз не нужно. Ячейки можно перезаписывать другими символами, но дисплей сразу обновляется, т.е. если символ под номером N выведен на дисплей, а затем в эту же ячейку был загружен другой символ - старый символ поменяется на новый. Ячейки сбрасываются при перезагрузке и повторной инициализации дисплея.

Для загрузки символа используются несколько форматов и функций:

```cpp
// 1. классический вариант - 8 байт строками (совместим с другими библиотеками)
const uint8_t smile[8] = {0x00, 0x0a, 0x0a, 0x00, 0x11, 0x0e, 0x00, 0x00};
lcd.createChar(0, smile);   // загрузка в 0
lcd.write(0);               // вывод 0
lcd.write(0);               // вывод 0

// 2. самый лёгкий вариант - 8 байт строками, упакованы в 64 бит
lcd.createChar(1, 0x40a110a0400);   // загрузка в 1
lcd.write(1);                       // вывод в 1

// можно сохранить и использовать по имени
// const uint64_t square = 0x40a110a0400;
// lcd.createChar(1, square);

// 3. компактный вариант - 5 байт столбцами (удобно для хранения в таблицах шрифтов)
const uint8_t spiral[5] = {0x7d, 0x45, 0x5d, 0x41, 0x7f};
lcd.createCharCols(2, spiral);
lcd.write(2);
```

Рекомендуется использовать вариант с 64-битным кодом, т.к. он оптимизируется компилятором и не занимает места в оперативной памяти!

> [!TIP]
> В библиотеке в папке utils имеется встроенная утилита для генерации кастомных символов с выводом во все форматы

### UTF-8
Библиотека декодирует 1- и 2-байтные UTF-8 символы из обычных строк и поддерживает вывод кириллицы и других языков, для этого есть 2 механизма и типа таблиц:

- **Аппаратная поддержка**: у дисплея есть своя таблица символов, обычно это ASCII + наборы для разных языковых групп, например есть дисплеи с аппаратной поддержкой кириллицы. В библиотеке в таблице типа `RomGlyph` можно задать соответствие между символом и его кодом в таблице дисплея. Таким образом библиотека может аппаратно выводить символы, похожие по написанию на английские, а также символы из таблицы дисплея
- **Программная поддержка**: в таблице типа `BitmapGlyph` можно задать битмапы для своего набора символов. Библиотека будет автоматически загружать их через `createChar` для всех новых уникальных символов, отправляемых на печать
- Формат и примеры таблиц можно посмотреть в файле glyphs.h

Это позволяет печатать на любом языке через единый механизм печати `print("строка")`, но с некоторыми ограничениями. Оба механизма работают одновременно, поэтому например для программной кириллицы используются обе таблицы: сначала библиотека ищет в таблице одинаковые по написанию символы из памяти дисплея (например русская В это английская B), если находит - печатает его. Если не находит - ищет в реестре текущих кастомных символов. Если находит - выводит его, если не находит - ищет в таблице битмап, загружает его, сохраняет в реестр и выводит:

- По умолчанию используются все 8 слотов для кастомных символов. Для использования этой версии совместно со своими кастомными символами нужно "ограничить" библиотеку при помощи `setGlyphSlots(N)` - она использует ячейки от 0 до установленного лимита (минус один)
- Количество слотов определяет, сколько уникальных UTF-8 символов, не совпадающих с английскими, может находиться на дисплее одновременно. При использовании поверх текущего лимита будут перезаписываться самые старые символы, а если они в этот момент выведены на дисплей - они изменятся на новые

```cpp
lcd.setGlyphSlots(6);
// UTF-8 использует 0..5
// слоты 6..7 свободны для createChar
```

Если занятые UTF-8 слоты меняются вручную через `createChar()`, после этого нужно вызвать `resetGlyphs()`.

По умолчанию в `GyverLCD_UTF8` подключена **программная** поддержка кириллицы (RU+BY+UKR) + символ градуса, т.к. это самый распространённый дисплей:

```cpp
#include <GyverLCD_UTF8.h>
GyverLCD_UTF8 lcd(0x27, 16, 2);
// GyverLCD_UTF8 lcd(0x27, 16, 2, GTABLE_CYR_SOFT);  // так по умолчанию

lcd.print("Привет 23°C");
```

Для дисплеев с **аппаратной** поддержкой кириллицы нужно передать таблицу `GTABLE_CYR_HARD`:

```cpp
GyverLCD_UTF8 lcd(0x27, 16, 2, GTABLE_CYR_HARD);
```

Эта таблица вообще не содержит программных символов - вся кириллица выводится дисплеем, кастом символы не используются.

## Настройка
### Pin map
Настройка распиновки находится во внутреннем файле `lcdpins.h`. Все define задаются **до** подключения заголовка GyverLCD.

По умолчанию используется распространённая разводка:
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

Готовый вариант:
```cpp
#define GLCD_MAP_MJKDZ
#include <GyverLCD.h>
```

Доступны `GLCD_MAP_LCM1602`, `GLCD_MAP_MJKDZ`, `GLCD_MAP_LCDXIO`.

Для своей разводки:
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

Если подсветка не подключена к expander:
```cpp
#define GLCD_PIN_BL GLCD_NO_PIN
```

### Тайминги
По умолчанию используются безопасные задержки: 45 мкс для обычной команды и 2000 мкс для `clear/home`.

```cpp
#define GLCD_FAST
#include <GyverLCD.h>
```

Свои значения:
```cpp
#define GLCD_COMMAND_US 50
#define GLCD_CLEAR_US 2200
#include <GyverLCD.h>
```

`setBusClock(hz)` сообщает фактическую частоту I2C, при значениях больше 1 МГц библиотека будет дополнительно ждать несколько микросекунд для команд.
```cpp
Wire.setClock(1000000);
lcd.setBusClock(1000000);
```

### Бенчмарк (мс)
| Библиотека        |     I²C | init | cursor | print | clear | create |
| ----------------- | ------: | ---: | -----: | ----: | ----: | -----: |
| LiquidCrystal_I2C | 100 кГц | 1074 |   1.50 |  1.50 |  3.50 |   14.0 |
| LiquidCrystal_I2C | 400 кГц | 1074 |   1.50 |  1.50 |  3.50 |   14.0 |
| LiquidCrystal_I2C | 800 кГц | 1074 |   1.50 |  1.50 |  3.50 |   14.0 |
| GyverLCD          | 100 кГц |   64 |   0.25 |  0.80 |  2.80 |    7.8 |
| GyverLCD          | 400 кГц |   60 |   0.08 |  0.30 |  2.30 |    2.5 |
| GyverLCD          | 800 кГц |   60 |   0.07 |  0.20 |  2.20 |    2.1 |

## Пример
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

## Установка
- Библиотеку можно найти по названию **GyverLCD** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverLCD/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>

## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код

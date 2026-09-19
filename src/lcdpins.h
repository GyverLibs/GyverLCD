#pragma once
#ifndef GLCD_NO_PIN
#define GLCD_NO_PIN 0xFF
#endif

// готовые варианты разводки backpack
#if defined(GLCD_MAP_LCM1602)
#ifndef GLCD_PIN_RS
#define GLCD_PIN_RS 0
#endif
#ifndef GLCD_PIN_RW
#define GLCD_PIN_RW 1
#endif
#ifndef GLCD_PIN_EN
#define GLCD_PIN_EN 2
#endif
#ifndef GLCD_PIN_BL
#define GLCD_PIN_BL 3
#endif
#ifndef GLCD_PIN_D4
#define GLCD_PIN_D4 4
#endif
#ifndef GLCD_PIN_D5
#define GLCD_PIN_D5 5
#endif
#ifndef GLCD_PIN_D6
#define GLCD_PIN_D6 6
#endif
#ifndef GLCD_PIN_D7
#define GLCD_PIN_D7 7
#endif
#ifndef GLCD_BL_ACTIVE_HIGH
#define GLCD_BL_ACTIVE_HIGH 0
#endif
#elif defined(GLCD_MAP_MJKDZ)
#ifndef GLCD_PIN_RS
#define GLCD_PIN_RS 6
#endif
#ifndef GLCD_PIN_RW
#define GLCD_PIN_RW 5
#endif
#ifndef GLCD_PIN_EN
#define GLCD_PIN_EN 4
#endif
#ifndef GLCD_PIN_BL
#define GLCD_PIN_BL 7
#endif
#ifndef GLCD_PIN_D4
#define GLCD_PIN_D4 0
#endif
#ifndef GLCD_PIN_D5
#define GLCD_PIN_D5 1
#endif
#ifndef GLCD_PIN_D6
#define GLCD_PIN_D6 2
#endif
#ifndef GLCD_PIN_D7
#define GLCD_PIN_D7 3
#endif
#ifndef GLCD_BL_ACTIVE_HIGH
#define GLCD_BL_ACTIVE_HIGH 0
#endif
#elif defined(GLCD_MAP_LCDXIO)
#ifndef GLCD_PIN_RS
#define GLCD_PIN_RS 4
#endif
#ifndef GLCD_PIN_RW
#define GLCD_PIN_RW 5
#endif
#ifndef GLCD_PIN_EN
#define GLCD_PIN_EN 6
#endif
#ifndef GLCD_PIN_BL
#define GLCD_PIN_BL GLCD_NO_PIN
#endif
#ifndef GLCD_PIN_D4
#define GLCD_PIN_D4 0
#endif
#ifndef GLCD_PIN_D5
#define GLCD_PIN_D5 1
#endif
#ifndef GLCD_PIN_D6
#define GLCD_PIN_D6 2
#endif
#ifndef GLCD_PIN_D7
#define GLCD_PIN_D7 3
#endif
#ifndef GLCD_BL_ACTIVE_HIGH
#define GLCD_BL_ACTIVE_HIGH 1
#endif
#else
// стандартная разводка YwRobot / DFRobot / SainSmart / Funduino
#ifndef GLCD_PIN_RS
#define GLCD_PIN_RS 0
#endif
#ifndef GLCD_PIN_RW
#define GLCD_PIN_RW 1
#endif
#ifndef GLCD_PIN_EN
#define GLCD_PIN_EN 2
#endif
#ifndef GLCD_PIN_BL
#define GLCD_PIN_BL 3
#endif
#ifndef GLCD_PIN_D4
#define GLCD_PIN_D4 4
#endif
#ifndef GLCD_PIN_D5
#define GLCD_PIN_D5 5
#endif
#ifndef GLCD_PIN_D6
#define GLCD_PIN_D6 6
#endif
#ifndef GLCD_PIN_D7
#define GLCD_PIN_D7 7
#endif
#ifndef GLCD_BL_ACTIVE_HIGH
#define GLCD_BL_ACTIVE_HIGH 1
#endif
#endif

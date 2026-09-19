#pragma once
#include <stdint.h>

// короткие тайминги по даташиту без дополнительного запаса
#ifdef GLCD_FAST
#ifndef GLCD_COMMAND_US
#define GLCD_COMMAND_US 37
#endif
#ifndef GLCD_CLEAR_US
#define GLCD_CLEAR_US 1520
#endif
#else
#ifndef GLCD_COMMAND_US
#define GLCD_COMMAND_US 45
#endif
#ifndef GLCD_CLEAR_US
#define GLCD_CLEAR_US 2000
#endif
#endif

// максимальная частота I2C, при которой транспорт сам обеспечивает задержку обычной команды
static constexpr uint32_t GLCD_NO_WAIT_MAX_HZ = 54000000UL / GLCD_COMMAND_US;

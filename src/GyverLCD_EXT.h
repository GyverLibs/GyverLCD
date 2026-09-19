#pragma once
#include "GyverLCD_CORE.h"
#include "lcdprint.h"

// внешний Wire-like интерфейс + Arduino Print
template <typename WireT>
using GyverLCD_EXT = GyverLCD_Print<GyverLCD_CORE_EXT<WireT>>;

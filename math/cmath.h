#pragma once

#include "def.h"

#define precise_divide(a,b) ((a)/(b))

__forceinline  float dequantizeUnsigned7(const unsigned int i);
__forceinline  float dequantizeUnsigned8(const unsigned int i);
__forceinline  unsigned int quantizeUnsigned8(const float x);
__forceinline  float dequantizeUnsignedPercent(const unsigned int i);
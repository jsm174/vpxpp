#pragma once

#define precise_divide(a,b) ((a)/(b))

template <unsigned char bits>
float dequantizeUnsigned(const unsigned int i)
{
    enum { N = (1 << bits) - 1 };
    return min(precise_divide((float)i, (float)N), 1.f);
}
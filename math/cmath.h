#pragma once

#define precise_divide(a,b) ((a)/(b))

template <unsigned char bits>
float dequantizeUnsigned(const unsigned int i)
{
    enum { N = (1 << bits) - 1 };
    return min(precise_divide((float)i, (float)N), 1.f);
}

template <unsigned char bits> // bits to map to
unsigned int quantizeUnsigned(const float x)
{
    enum { N = (1 << bits) - 1, Np1 = (1 << bits) };
    assert(x >= 0.f);
    return min((unsigned int)(x * (float)Np1), (unsigned int)N);
}
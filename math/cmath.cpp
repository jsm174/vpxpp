#include "cmath.h"
#include "def.h"

__forceinline float dequantizeUnsigned8(const unsigned int i)
{
    enum { N = (1 << 8) - 1 };
    return min(precise_divide((float)i, (float)N), 1.f);
}

__forceinline float dequantizeUnsigned7(const unsigned int i)
{
    enum { N = (1 << 7) - 1 };
    return min(precise_divide((float)i, (float)N), 1.f);
}

__forceinline unsigned int quantizeUnsigned8(const float x)
{
    enum { N = (1 << 8) - 1, Np1 = (1 << 8) };
    return min((unsigned int)(x * (float)Np1), (unsigned int)N);
}

__forceinline float dequantizeUnsignedPercent(const unsigned int i)
{
    enum { N = 100 };
    return min(precise_divide((float)i, (float)N), 1.f); 
}
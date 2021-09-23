#pragma once

#include <assert.h>

#ifndef _MSC_VER
// https://stackoverflow.com/questions/44624759/linking-object-files-having-common-header-files-to-main-gives-duplicate-symbols
#define __forceinline __attribute__((always_inline)) inline
#endif

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

__forceinline float min(const float x, const float y)
{
	return x < y ? x : y;
}

__forceinline float max(const float x, const float y)
{
	return x < y ? y : x;
}

__forceinline double min(const double x, const double y)
{
	return x < y ? x : y;
}

__forceinline double max(const double x, const double y)
{
	return x < y ? y : x;
}

__forceinline int min(const int x, const int y)
{
	return x < y ? x : y;
}

__forceinline int max(const int x, const int y)
{
	return x < y ? y : x;
}

__forceinline long max(const long x, const long y)
{
	return x < y ? y : x;
}

__forceinline unsigned int min(const unsigned int x, const unsigned int y)
{
	return x < y ? x : y;
}

// TODO:
//
//__forceinline unsigned int min(const DWORD x, const DWORD y)
//{
//   return x < y ? x : y;
//}

__forceinline unsigned int max(const unsigned int x, const unsigned int y)
{
	return x < y ? y : x;
}

__forceinline unsigned long long min(const unsigned long long x, const unsigned long long y)
{
	return x < y ? x : y;
}

__forceinline unsigned long long max(const unsigned long long x, const unsigned long long y)
{
	return x < y ? y : x;
}

__forceinline float clamp(const float x, const float min, const float max)
{
	if (x < min)
	{
		return min;
	}
	else if (x > max)
	{
		return max;
	}

	return x;
}

template <unsigned char bits>
__forceinline float dequantizeUnsigned(const unsigned int i)
{
	enum
	{
		N = (1 << bits) - 1
	};
	return min(precise_divide((float)i, (float)N), 1.f);
}

template <unsigned char bits>
__forceinline unsigned int quantizeUnsigned(const float x)
{
	enum
	{
		N = (1 << bits) - 1,
		Np1 = (1 << bits)
	};
	assert(x >= 0.f);
	return min((unsigned int)(x * (float)Np1), (unsigned int)N);
}

__forceinline float dequantizeUnsignedPercent(const unsigned int i)
{
	enum
	{
		N = 100
	};
	return min(precise_divide((float)i, (float)N), 1.f);
}

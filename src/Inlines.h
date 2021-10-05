#pragma once

#include <assert.h>
#include <vector>

#ifndef _MSC_VER
// https://stackoverflow.com/questions/44624759/linking-object-files-having-common-header-files-to-main-gives-duplicate-symbols
#define __forceinline __attribute__((always_inline)) inline
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define precise_divide(a, b) ((a) / (b))

__forceinline float radical_inverse(unsigned int v)
{
	v = (v << 16) | (v >> 16);
	v = ((v & 0x55555555u) << 1) | ((v & 0xAAAAAAAAu) >> 1);
	v = ((v & 0x33333333u) << 2) | ((v & 0xCCCCCCCCu) >> 2);
	v = ((v & 0x0F0F0F0Fu) << 4) | ((v & 0xF0F0F0F0u) >> 4);
	v = ((v & 0x00FF00FFu) << 8) | ((v & 0xFF00FF00u) >> 8);
	return (float)v * 0.00000000023283064365386962890625f;
}

__forceinline float invGammaApprox(const float c)
{
	return c * (c * (c * 0.305306011f + 0.682171111f) + 0.012522878f);
}

__forceinline float gammaApprox(const float c)
{
	const float t0 = sqrtf(c);
	const float t1 = sqrtf(t0);
	const float t2 = sqrtf(t1);
	return 0.662002687f * t0 + 0.684122060f * t1 - 0.323583601f * t2 - 0.0225411470f * c;
}

__forceinline float acos_approx_divPI(const float v)
{
	const float x = fabsf(v);
	const float res = ((float)(-0.155972 / M_PI) * x + (float)(1.56467 / M_PI)) * sqrtf(1.0f - x);
	return (v >= 0.f) ? res : 1.f - res;
}

__forceinline float atan2_approx_div2PI(const float y, const float x)
{
	const float abs_y = fabsf(y);
	const float abs_x = fabsf(x);

	if (abs_x < 0.0000001f && abs_y < 0.0000001f)
	{
		return 0.f;
	}

	const float r = (abs_x - abs_y) / (abs_x + abs_y);
	const float angle = ((x < 0.f) ? (float)(3. / 8.) : (float)(1. / 8.)) + ((float)(0.211868 / (2. * M_PI)) * r * r - (float)(0.987305 / (2. * M_PI))) * ((x < 0.f) ? -r : r);
	return (y < 0.f) ? -angle : angle;
}

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

__forceinline int float_as_int(const float x)
{
	union
	{
		float f;
		int i;
	} uc;
	uc.f = x;
	return uc.i;
}

__forceinline float int_as_float(const int i)
{
	union
	{
		int i;
		float f;
	} iaf;
	iaf.i = i;
	return iaf.f;
}

__forceinline bool infNaN(const float a)
{
	return ((float_as_int(a) & 0x7F800000) == 0x7F800000);
}

template <typename T>
__forceinline int FindIndexOf(const std::vector<T>& v, const T& val)
{
	typename std::vector<T>::const_iterator it = std::find(v.begin(), v.end(), val);
	if (it != v.end())
		return (int)(it - v.begin());
	else
		return -1;
}
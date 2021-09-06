#pragma once

#include <string>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define __forceinline __attribute__((always_inline))

__forceinline float min(const float x, const float y);
__forceinline float max(const float x, const float y);
__forceinline double min(const double x, const double y);
__forceinline double max(const double x, const double y);
__forceinline int min(const int x, const int y);
__forceinline int max(const int x, const int y);
__forceinline long max(const long x, const long y);
__forceinline unsigned int min(const unsigned int x, const unsigned int y);
// TODO: __forceinline unsigned int min(const DWORD x, const DWORD y);
__forceinline unsigned int max(const unsigned int x, const unsigned int y);
__forceinline unsigned long long min(const unsigned long long x, const unsigned long long y);
__forceinline unsigned long long max(const unsigned long long x, const unsigned long long y);
__forceinline float clamp(const float x, const float min, const float max);

typedef float D3DVALUE;

#define MAXNAMEBUFFER 32
#define MAXSTRING 1024

#define EDITOR_BG_WIDTH 1000
#define EDITOR_BG_HEIGHT 750

class Vertex3D_NoTex2
{
public:
	D3DVALUE x;
	D3DVALUE y;
	D3DVALUE z;

	D3DVALUE nx;
	D3DVALUE ny;
	D3DVALUE nz;

	D3DVALUE tu;
	D3DVALUE tv;
};

class Vertex3D_TexelOnly
{
public:
	D3DVALUE x;
	D3DVALUE y;
	D3DVALUE z;

	D3DVALUE tu;
	D3DVALUE tv;
};

void TitleFromFilename(const std::string& szfilename, std::string& sztitle);
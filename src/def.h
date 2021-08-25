#pragma once

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

typedef float D3DVALUE;

#define MAXNAMEBUFFER 32

class Vertex3D_NoTex2 
{
public:
	// Position
	D3DVALUE x;
	D3DVALUE y;
	D3DVALUE z;

	// Normals
	D3DVALUE nx; // w component if MY_D3DTRANSFORMED_NOTEX2_VERTEX
	D3DVALUE ny; // tex coord (1) if MY_D3DTRANSFORMED_NOTEX2_VERTEX
	D3DVALUE nz; // dto.

	// Texture coordinates (0)
	D3DVALUE tu;
	D3DVALUE tv;
};
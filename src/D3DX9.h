#pragma once

#include "misc.h"

typedef float D3DVALUE;

typedef DWORD D3DCOLOR;

typedef struct _D3DRECT
{
	long x1;
	long y1;
	long x2;
	long y2;
} D3DRECT;

typedef enum _D3DTRANSFORMSTATETYPE
{
	D3DTS_VIEW = 2,
	D3DTS_PROJECTION = 3,
	D3DTS_TEXTURE0 = 16,
	D3DTS_TEXTURE1 = 17,
	D3DTS_TEXTURE2 = 18,
	D3DTS_TEXTURE3 = 19,
	D3DTS_TEXTURE4 = 20,
	D3DTS_TEXTURE5 = 21,
	D3DTS_TEXTURE6 = 22,
	D3DTS_TEXTURE7 = 23,

	D3DTS_FORCE_DWORD = 0x7fffffff
} D3DTRANSFORMSTATETYPE;

typedef const char* D3DXHANDLE;

#define D3DTS_WORLDMATRIX(index) (D3DTRANSFORMSTATETYPE)(index + 256)

#define D3DTS_WORLD D3DTS_WORLDMATRIX(0)

typedef struct
{
	DWORD X;
	DWORD Y;
	DWORD Width;
	DWORD Height;
	float MinZ;
	float MaxZ;
} D3DVIEWPORT9;

typedef D3DVIEWPORT9 ViewPort;

typedef struct _D3DMATRIX
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
} D3DMATRIX;

typedef struct D3DXFLOAT16
{
	WORD Value;
} D3DXFLOAT16, *LPD3DXFLOAT16;

typedef struct D3DXMATRIX : public D3DMATRIX
{
public:
	D3DXMATRIX(){};
	D3DXMATRIX(const float*);
	D3DXMATRIX(const D3DMATRIX&);
	D3DXMATRIX(const D3DXFLOAT16*);
	D3DXMATRIX(float _11, float _12, float _13, float _14,
			   float _21, float _22, float _23, float _24,
			   float _31, float _32, float _33, float _34,
			   float _41, float _42, float _43, float _44);

	// access grants
	float& operator()(UINT Row, UINT Col);
	float operator()(UINT Row, UINT Col) const;

	// casting operators
	operator float*();
	operator const float*() const;

	// assignment operators
	D3DXMATRIX& operator*=(const D3DXMATRIX&);
	D3DXMATRIX& operator+=(const D3DXMATRIX&);
	D3DXMATRIX& operator-=(const D3DXMATRIX&);
	D3DXMATRIX& operator*=(float);
	D3DXMATRIX& operator/=(float);

	// unary operators
	D3DXMATRIX operator+() const;
	D3DXMATRIX operator-() const;

	// binary operators
	D3DXMATRIX operator*(const D3DXMATRIX&) const;
	D3DXMATRIX operator+(const D3DXMATRIX&) const;
	D3DXMATRIX operator-(const D3DXMATRIX&) const;
	D3DXMATRIX operator*(float) const;
	D3DXMATRIX operator/(float) const;

	friend D3DXMATRIX operator*(float, const D3DXMATRIX&);

	bool operator==(const D3DXMATRIX&) const;
	bool operator!=(const D3DXMATRIX&) const;

} D3DXMATRIX, *LPD3DXMATRIX;

D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* pout, const D3DXMATRIX* pm1, const D3DXMATRIX* pm2);

inline D3DXMATRIX D3DXMATRIX::operator*(const D3DXMATRIX& mat) const
{
	D3DXMATRIX buf;
	D3DXMatrixMultiply(&buf, this, &mat);
	return buf;
}

inline D3DXMATRIX::D3DXMATRIX(const D3DMATRIX& mat)
{
	memcpy(&_11, &mat, sizeof(D3DXMATRIX));
}

inline D3DXMATRIX::D3DXMATRIX(float f11, float f12, float f13, float f14,
							  float f21, float f22, float f23, float f24,
							  float f31, float f32, float f33, float f34,
							  float f41, float f42, float f43, float f44)
{
	_11 = f11;
	_12 = f12;
	_13 = f13;
	_14 = f14;
	_21 = f21;
	_22 = f22;
	_23 = f23;
	_24 = f24;
	_31 = f31;
	_32 = f32;
	_33 = f33;
	_34 = f34;
	_41 = f41;
	_42 = f42;
	_43 = f43;
	_44 = f44;
}

typedef struct D3DVECTOR
{
	float x;
	float y;
	float z;
} D3DVECTOR;

typedef struct D3DXVECTOR4
{
public:
	D3DXVECTOR4(){};
	D3DXVECTOR4(const float*);
	D3DXVECTOR4(const D3DXFLOAT16*);
	D3DXVECTOR4(const D3DVECTOR& xyz, float w);
	D3DXVECTOR4(float x, float y, float z, float w);

	// casting
	operator float*();
	operator const float*() const;

	// assignment operators
	D3DXVECTOR4& operator+=(const D3DXVECTOR4&);
	D3DXVECTOR4& operator-=(const D3DXVECTOR4&);
	D3DXVECTOR4& operator*=(float);
	D3DXVECTOR4& operator/=(float);

	// unary operators
	D3DXVECTOR4 operator+() const;
	D3DXVECTOR4 operator-() const;

	// binary operators
	D3DXVECTOR4 operator+(const D3DXVECTOR4&) const;
	D3DXVECTOR4 operator-(const D3DXVECTOR4&) const;
	D3DXVECTOR4 operator*(float) const;
	D3DXVECTOR4 operator/(float) const;

	friend D3DXVECTOR4 operator*(float, const D3DXVECTOR4&);

	bool operator==(const D3DXVECTOR4&) const;
	bool operator!=(const D3DXVECTOR4&) const;

public:
	float x, y, z, w;
} D3DXVECTOR4, *LPD3DXVECTOR4;

inline D3DXVECTOR4::D3DXVECTOR4(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

typedef struct D3DXVECTOR4 vec4;

inline D3DXVECTOR4 convertColor(const COLORREF c, const float w = 1.0f)
{
	const float r = (float)(c & 255) * (float)(1.0 / 255.0);
	const float g = (float)(c & 65280) * (float)(1.0 / 65280.0);
	const float b = (float)(c & 16711680) * (float)(1.0 / 16711680.0);
	return D3DXVECTOR4(r, g, b, w);
}

D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pout, float fovy, float aspect, float zn, float zf);
D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pout);

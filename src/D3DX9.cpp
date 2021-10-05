#include "D3DX9.h"

#include <math.h>

D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* pout, const D3DXMATRIX* pm1, const D3DXMATRIX* pm2)
{
	D3DXMATRIX out;
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			out.m[i][j] = pm1->m[i][0] * pm2->m[0][j] + pm1->m[i][1] * pm2->m[1][j] + pm1->m[i][2] * pm2->m[2][j] + pm1->m[i][3] * pm2->m[3][j];
		}
	}

	*pout = out;
	return pout;
}

// https://github.com/wine-mirror/wine/blob/master/dlls/d3dx9_36/math.c#L479

D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pout, float fovy, float aspect, float zn, float zf)
{
	D3DXMatrixIdentity(pout);
	pout->m[0][0] = 1.0f / (aspect * tanf(fovy / 2.0f));
	pout->m[1][1] = 1.0f / tanf(fovy / 2.0f);
	pout->m[2][2] = zf / (zf - zn);
	pout->m[2][3] = 1.0f;
	pout->m[3][2] = (zf * zn) / (zn - zf);
	pout->m[3][3] = 0.0f;
	return pout;
}

// https://github.com/wine-mirror/wine/blob/master/include/d3dx9math.inl#L1192

D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pout)
{
	if (!pout)
		return NULL;
	(*pout).m[0][1] = 0.0f;
	(*pout).m[0][2] = 0.0f;
	(*pout).m[0][3] = 0.0f;
	(*pout).m[1][0] = 0.0f;
	(*pout).m[1][2] = 0.0f;
	(*pout).m[1][3] = 0.0f;
	(*pout).m[2][0] = 0.0f;
	(*pout).m[2][1] = 0.0f;
	(*pout).m[2][3] = 0.0f;
	(*pout).m[3][0] = 0.0f;
	(*pout).m[3][1] = 0.0f;
	(*pout).m[3][2] = 0.0f;
	(*pout).m[0][0] = 1.0f;
	(*pout).m[1][1] = 1.0f;
	(*pout).m[2][2] = 1.0f;
	(*pout).m[3][3] = 1.0f;
	return pout;
}

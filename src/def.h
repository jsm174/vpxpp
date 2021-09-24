#pragma once

#include <string>
#include <vector>

typedef float D3DVALUE;

typedef struct
{
	long left;
	long top;
	long right;
	long bottom;
} RECT;

#define ANGTORAD(x) ((x) * (float)(M_PI / 180.0))

#define MAXNAMEBUFFER 32
#define MAXSTRING 1024

#define EDITOR_BG_WIDTH 1000
#define EDITOR_BG_HEIGHT 750

#define DEFAULT_PLAYER_WIDTH 1024

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

template <typename T>
inline int FindIndexOf(const std::vector<T>& v, const T& val)
{
	typename std::vector<T>::const_iterator it = std::find(v.begin(), v.end(), val);
	if (it != v.end())
		return (int)(it - v.begin());
	else
		return -1;
}

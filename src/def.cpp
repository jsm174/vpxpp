#include "def.h"

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

void TitleFromFilename(const std::string& szfilename, std::string& sztitle)
{
	const int len = (int)szfilename.length();

	int begin;
	for (begin = len; begin >= 0; begin--)
	{
		if (szfilename[begin] == '\\')
		{
			begin++;
			break;
		}
	}

	int end;
	for (end = len; end >= 0; end--)
	{
		if (szfilename[end] == '.')
			break;
	}

	if (end == 0)
		end = len - 1;

	const char* szT = szfilename.c_str() + begin;
	int count = end - begin;

	sztitle.clear();
	while (count--)
	{
		sztitle.push_back(*szT++);
	}
}

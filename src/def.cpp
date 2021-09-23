#include "def.h"

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

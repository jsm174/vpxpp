#pragma once

#include "misc.h"

class IScriptable
{
public:
	IScriptable();

	wchar_t m_wzName[MAXNAMEBUFFER];
};
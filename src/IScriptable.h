#pragma once

#include "misc.h"

class ISelect;

class IScriptable
{
public:
	IScriptable();

	virtual ISelect* GetISelect() = 0;
	virtual const ISelect* GetISelect() const = 0;

	virtual wchar_t* get_Name() = 0;

	wchar_t m_wzName[MAXNAMEBUFFER];
};
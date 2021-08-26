#pragma once

#include "pole.h"
#include "misc.h"

class PinTable;

class IEditable
{
public:
	IEditable();

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version) = 0;
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName) = 0;
};
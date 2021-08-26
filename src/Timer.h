#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Timer : public IEditable
{
public:
	Timer();
	~Timer();

	static const ItemTypeEnum ItemType = eItemTimer;
	static const int TypeNameID = 0;
	static const int ToolID = 0;
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Timer* COMCreate()
	{
		return new Timer();
	}

	static IEditable* COMCreateEditable()
	{
		return static_cast<IEditable*>(COMCreate());
	}

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
};
#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Plunger : public IEditable
{
public:
	Plunger();
	~Plunger();

	static const ItemTypeEnum ItemType = eItemPlunger;
	static const int TypeNameID = 0;
	static const int ToolID = 0;
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Plunger* COMCreate()
	{
		return new Plunger();
	}

	static IEditable* COMCreateEditable()
	{
		return static_cast<IEditable*>(COMCreate());
	}

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
};

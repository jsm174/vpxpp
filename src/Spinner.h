#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Spinner :
	public IEditable
{
public:

	Spinner();
	~Spinner();

	static const ItemTypeEnum ItemType = eItemSpinner;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Spinner* COMCreate()
	{
		return new Spinner();
	} 

	static IEditable* COMCreateEditable()   { 
		return static_cast<IEditable*>(COMCreate()); 
	}

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version);
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName);
};
#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Surface :
	public IEditable
{
public:

	Surface();
	~Surface();

	static const ItemTypeEnum ItemType = eItemSurface;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Surface* COMCreate()
	{
		return new Surface();
	} 

	static IEditable* COMCreateEditable()   { 
		return static_cast<IEditable*>(COMCreate()); 
	}
};
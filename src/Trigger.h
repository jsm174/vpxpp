#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Trigger :
	public IEditable
{
public:

	Trigger();
	~Trigger();

	static const ItemTypeEnum ItemType = eItemTrigger;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Trigger* COMCreate()
	{
		return new Trigger();
	} 

	static IEditable* COMCreateEditable()   { 
		return static_cast<IEditable*>(COMCreate()); 
	}
};
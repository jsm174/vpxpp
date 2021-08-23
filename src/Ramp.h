#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Ramp :
    public IEditable
{
public:

    Ramp();
    ~Ramp();

	static const ItemTypeEnum ItemType = eItemRamp;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Ramp* COMCreate()
    {
		return new Ramp();
	} 

	static IEditable* COMCreateEditable()   { 
	return static_cast<IEditable*>(COMCreate()); 
	}
};
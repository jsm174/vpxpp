#pragma once

#include "IEditable.h"

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
};
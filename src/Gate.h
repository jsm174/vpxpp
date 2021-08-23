#pragma once

#include "IEditable.h"

class Gate :
    public IEditable
{
public:

    Gate();
    ~Gate();

	static const ItemTypeEnum ItemType = eItemGate;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
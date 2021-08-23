#pragma once

#include "IEditable.h"

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
};
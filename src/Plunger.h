#pragma once

#include "IEditable.h"

class Plunger :
    public IEditable
{
public:

    Plunger();
    ~Plunger();

	static const ItemTypeEnum ItemType = eItemPlunger;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
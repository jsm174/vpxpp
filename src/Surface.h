#pragma once

#include "IEditable.h"

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
};
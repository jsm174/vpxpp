#pragma once

#include "IEditable.h"

class Primitive :
    public IEditable
{
public:

    Primitive();
    ~Primitive();

	static const ItemTypeEnum ItemType = eItemPrimitive;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
#pragma once

#include "IEditable.h"

class Rubber :
    public IEditable
{
public:

    Rubber();
    ~Rubber();

	static const ItemTypeEnum ItemType = eItemRubber;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
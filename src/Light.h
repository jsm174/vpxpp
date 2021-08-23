#pragma once

#include "IEditable.h"

class Light :
    public IEditable
{
public:

    Light();
    ~Light();

	static const ItemTypeEnum ItemType = eItemLight;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
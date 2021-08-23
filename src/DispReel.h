#pragma once

#include "IEditable.h"

class DispReel :
    public IEditable
{
public:

    DispReel();
    ~DispReel();

	static const ItemTypeEnum ItemType = eItemDispReel;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
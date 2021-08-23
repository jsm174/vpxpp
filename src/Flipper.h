#pragma once

#include "IEditable.h"

class Flipper :
    public IEditable
{
public:

    Flipper();
    ~Flipper();

	static const ItemTypeEnum ItemType = eItemFlipper;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
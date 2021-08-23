#pragma once

#include "IEditable.h"

class Flasher :
    public IEditable
{
public:

    Flasher();
    ~Flasher();

	static const ItemTypeEnum ItemType = eItemFlasher;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
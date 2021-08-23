#pragma once

#include "IEditable.h"

class Bumper :
    public IEditable
{
public:

    Bumper();
    ~Bumper();

	static const ItemTypeEnum ItemType = eItemBumper;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
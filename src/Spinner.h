#pragma once

#include "IEditable.h"

class Spinner :
    public IEditable
{
public:

    Spinner();
    ~Spinner();

	static const ItemTypeEnum ItemType = eItemSpinner;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
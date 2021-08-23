#pragma once

#include "IEditable.h"

class Decal :
    public IEditable
{
public:

    Decal();
    ~Decal();

	static const ItemTypeEnum ItemType = eItemDecal;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
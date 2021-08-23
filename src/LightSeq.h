#pragma once

#include "IEditable.h"

class LightSeq :
    public IEditable
{
public:

    LightSeq();
    ~LightSeq();

	static const ItemTypeEnum ItemType = eItemLightSeq;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;
};
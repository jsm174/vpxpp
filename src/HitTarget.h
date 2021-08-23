#pragma once

#include "IEditable.h"
#include "ISelect.h"

class HitTarget :
    public IEditable
{
public:

    HitTarget();
    ~HitTarget();

	static const ItemTypeEnum ItemType = eItemHitTarget;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static HitTarget* COMCreate()
    {
		return new HitTarget();
	} 

	static IEditable* COMCreateEditable()   { 
	return static_cast<IEditable*>(COMCreate()); 
	}
};
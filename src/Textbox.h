#pragma once

#include "IEditable.h"
#include "ISelect.h"

class Textbox :
	public IEditable
{
public:

	Textbox();
	~Textbox();

	static const ItemTypeEnum ItemType = eItemTextbox;
	static const int TypeNameID = 0;
	static const int ToolID = 0; 
	static const int CursorID = 0;
	static const unsigned AllowedViews = 1;

	static Textbox* COMCreate()
	{
		return new Textbox();
	} 

	static IEditable* COMCreateEditable()   { 	return static_cast<IEditable*>(COMCreate()); }
};
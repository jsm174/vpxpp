#pragma once

#include <map>

#include "IEditable.h"
#include "ISelect.h"
#include "PinTable.h"

typedef IEditable*(*CreateFuncType)();
typedef IEditable*(*CreateAndInitFuncType)(PinTable* pPinTable, float x, float y);

struct EditableInfo
{
	ItemTypeEnum type;
	int typeNameID;
	int toolID;
	int cursorID;
	unsigned int allowedViews;
	CreateFuncType createFunc;
	CreateAndInitFuncType createAndInitFunc;
};

class EditableRegistry
{
public:
	template <class T>
	static void RegisterEditable()
	{
		EditableInfo ei;
		ei.type = T::ItemType;
		ei.typeNameID = T::TypeNameID;
		ei.toolID = T::ToolID;
		ei.cursorID = T::CursorID;
		ei.allowedViews = T::AllowedViews;
		ei.createFunc = &T::COMCreateEditable;
		// TODO: ei.createAndInitFunc = &T::COMCreateAndInit;
		m_map[ei.type] = ei;
	}

	static IEditable* Create(ItemTypeEnum type)
	{
		return FindOrFail(type)->createFunc();
	}

	static IEditable* CreateAndInit(ItemTypeEnum type, PinTable* pinTable, float x, float y)
	{
		return FindOrFail(type)->createAndInitFunc(pinTable, x, y);
	}

	static int GetTypeNameStringID(ItemTypeEnum type)
	{
		return FindOrFail(type)->typeNameID;
	}

	static ItemTypeEnum TypeFromToolID(int toolID)
	{
		std::map<ItemTypeEnum, EditableInfo>::iterator it;
		for (it = m_map.begin(); it != m_map.end(); ++it)
		{
			if (it->second.toolID == toolID)
			return it->second.type;
		}
		return eItemInvalid;
	}

	static int GetCursorID(ItemTypeEnum type)
	{
		return FindOrFail(type)->cursorID;
	}

	static unsigned int GetAllowedViews(ItemTypeEnum type)
	{
		return FindOrFail(type)->allowedViews;
	}

private:
	static std::map<ItemTypeEnum, EditableInfo> m_map;

	static EditableInfo* FindOrFail(ItemTypeEnum type)
	{
		std::map<ItemTypeEnum, EditableInfo>::iterator it = m_map.find(type);
		if (it == m_map.end())
		{
			assert(false);
			return NULL;
		}
		else
		{
			return &it->second;
		}
	}
};
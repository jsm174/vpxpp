#pragma once

#include "BiffReader.h"
#include "IEditable.h"
#include "ILoadable.h"

class PinTable;

class ISelect : public ILoadable
{
public:
	ISelect();

	void GetTypeNameForType(const ItemTypeEnum type, wchar_t* const buf) const;

	virtual bool LoadToken(const int id, BiffReader* const pBiffReader);

	virtual PinTable* GetPTable() = 0;
	virtual const PinTable* GetPTable() const = 0;

	virtual IEditable* GetIEditable() = 0;
	virtual const IEditable* GetIEditable() const = 0;

	std::string m_layerName;

	bool m_dragging;
	bool m_markedForUndo;
	bool m_locked;
	bool m_isVisible;

	unsigned char m_oldLayerIndex;
};
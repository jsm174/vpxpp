#pragma once

#include "BiffReader.h"
#include "ILoadable.h"

class PinTable;

class ISelect : public ILoadable
{
public:
	ISelect();

	virtual PinTable* GetPTable() = 0;

	void GetTypeNameForType(const ItemTypeEnum type, wchar_t* const buf) const;

	virtual bool LoadToken(const int id, BiffReader* const pBiffReader);

	std::string m_layerName;

	bool m_dragging;
	bool m_markedForUndo;
	bool m_locked;
	bool m_isVisible;

	unsigned char m_oldLayerIndex;
};
#pragma once

#include "Vertex3Ds.h"

#include "misc.h"
#include "pole.h"

#include <vector>

class IHitable;
class ISelect;
class IScriptable;
class Collection;
class PinTable;

class IEditable
{
public:
	IEditable();
	virtual ~IEditable();

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version) = 0;
	virtual HRESULT InitPostLoad() = 0;
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName) = 0;

	virtual PinTable* GetPTable() = 0;
	virtual const PinTable* GetPTable() const = 0;
	virtual ISelect* GetISelect() = 0;
	virtual const ISelect* GetISelect() const = 0;
	virtual IHitable* GetIHitable();
	virtual const IHitable* GetIHitable() const;

	virtual ItemTypeEnum GetItemType() const = 0;
	virtual IScriptable* GetScriptable() = 0;

	void InitScript();

	virtual void GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D);

	std::vector<Collection*> m_vCollection;
	std::vector<int> m_viCollection;

	bool m_backglass;
};
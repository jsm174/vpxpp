#pragma once

#include "misc.h"
#include "pole.h"

#include "Vertex3Ds.h"

class PinTable;
class IHitable;

class IEditable
{
public:
	IEditable();
	virtual ~IEditable();

	virtual HRESULT InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version) = 0;
	virtual HRESULT InitPostLoad() = 0;
	virtual HRESULT InitVBA(bool fNew, int id, wchar_t* const wzName) = 0;

	virtual PinTable* GetPTable() = 0;
	virtual IHitable* GetIHitable();

	virtual ItemTypeEnum GetItemType() const = 0;

	void InitScript();

	virtual void GetBoundingVertices(std::vector<Vertex3Ds>& pvvertex3D);

	bool m_backglass;
};